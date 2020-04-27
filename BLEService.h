/*
    Edgar Ramirez-Villa
    BLE Service Header
    Senior Design Spring 2020
    Cubic Transportation
*/
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

//Global Var
std::unordered_map<std::string, int> desiredAddrs;
std::unordered_map<std::string, int> recentlyProcessedAddrs;

//initialize list of all beacons
void initDesiredAddrs();

//Function to reset ble services
void BLEReset();

//Parse param "terminalOutput" to a vector as Bluetooth addresses only
void parseToVector(std::string terminalOutput);

//Get terminal output resulting from param "cmd"
std::string GetStdoutFromCommand(std::string cmd);

//previous main function
//scans ble devices
std::string BLEService();

