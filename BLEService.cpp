#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
//#include "BLEDevices.h"


using namespace std;

void BLEReset(){
    system("sudo hciconfig hci0 down");
    system("sudo hciconfig hci0 up");
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
    
    string cmnd = GetStdoutFromCommand("sudo timeout -s INT 5s hcitool lescan");
    
    

    cout << "output: \n" << cmnd << endl;

    return 0;
    }
