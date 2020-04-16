#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

//#include "BLEDevices.h"


using namespace std;

vector<string> foundAddresses;

void BLEReset(){
    system("sudo hciconfig hci0 down");
    system("sudo hciconfig hci0 up");
}

//parses the terminal output by newline char first
//then parses those lines by space to separate the name and addr
void parseToVector(string terminalOutput) {
    stringstream ss(terminalOutput);
    string to;
    if(!terminalOutput.empty())
    {
        while(getline(ss,to,'\n')){
           stringstream to_ss(to);
           string finalAddress;
           if(!to.empty()){
               while(getline(to_ss,finalAddress,' ')){
                   if(finalAddress.size() == 17){
                   foundAddresses.push_back(finalAddress);
                   }
               }
           }
        }
    }
}

string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    BLEReset();
    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
    }
    return data;
    }

int main (){
    string desiredAddr = "C9:0E:DB:EA:12:02";
    string connectCommand = "sudo gatttool -b ";
    const char *command;
    const char *connect = "connect";

    string cmnd = GetStdoutFromCommand("sudo timeout -s INT 5s hcitool lescan");
    
    parseToVector(cmnd);

    for(int i = 0; i < foundAddresses.size(); i++){
        if(desiredAddr.compare(foundAddresses[i]) == 0){
            connectCommand += foundAddresses[i] + " -I";
            command = connectCommand.c_str();
            system(command);
        }
    }

    return 0;
    }
