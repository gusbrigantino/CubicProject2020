#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service. Not sure what this does
//BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
//BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

String id = "1234";

void setup() {
  Serial.begin(9600);
  while(!Serial); //what does this do? 
  
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    
    while (1);
  }
  
  BLE.setLocalName("Arduino");
  BLE.setAdvertisedService(ledService); //why? 

  //used to enavle the services board provides or interact with services a remote board provides 
  
  //add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  //enable BLE module: add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
  
  BLE.advertise();
  String address = BLE.address();

  Serial.print("Local address is: ");
  Serial.println(address); 
  Serial.println(id); 
  //if (switchCharacteristic.canWrite()) {
    //Serial.println("characteristic is writable");
  //}
  //else{
    //Serial.println("characteristic is not writable");
  //}

  //if (switchCharacteristic.canRead()) {
    //Serial.println("characteristic is readable");
  //}
  //else{
    //Serial.println("characteristic is not readable");
  //}
}

void loop() {

  // listen for BLE peripherals to connect:
    BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
    if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      if (switchCharacteristic.written()){
        if (switchCharacteristic.value()){
          Serial.print("RSSI = "); 
          Serial.println(BLE.rssi());
          Serial.println("Write value recieved"); 
        }
        else{
          Serial.println(F("---"));
        }
      }
    }
    
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
