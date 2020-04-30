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
#include <unordered_set> 

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define MAC_ADDR_LEN        17

#define MAC_INDEX_BT        2
#define RSSI_INDEX_BT       7

#define NULL_STR            "\0"

#define LESCAN              "sudo timeout -s INT 0.25s hcitool lescan"
#define BTMGMT_FIND         "sudo timeout -s INT 0.25s btmgmt find"

//Global Var
extern std::unordered_set<std::string> desiredAddrs;

//initialize list of all beacons
void InitDesiredAddrs();

//Function to reset ble services
void BLEReset();

//Parse param "terminalOutput" to a vector as Bluetooth addresses only
//previously parseToVector
void ParseHcitoolLescan(std::string terminalOutput);

//parses the terminal output from $ sudo btmgmt find
void ParseBtmgmtFind(std::string terminalOutput);

//Get terminal output resulting from param "cmd"
std::string GetStdoutFromCommand(std::string cmd);

//previous main function
//scans ble devices
std::string BLEService();

