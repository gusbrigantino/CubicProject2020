
#include "BLEService.h"


using namespace std;

vector<string> foundAddresses;
vector<string> approvedAddr;

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

//TODO: Replace string with vector<string> of all accetable addr
string desiredAddr = "04:91:62:97:8B:38";

string getBLEAddr(){
    //if(approvedAddr.size() > 0)
    return approvedAddr[0];

}

int main (){
    string connectCommand = "sudo gatttool -b ";
    //string connectCommand = "connect ";
    const char *command;
    const char *connect = "connect";

    string cmnd = GetStdoutFromCommand("sudo timeout -s INT 5s hcitool lescan");
    
    parseToVector(cmnd);

    for(int i = 0; i < foundAddresses.size(); i++){
        if(desiredAddr.compare(foundAddresses[i]) == 0){
            approvedAddr.push_back(desiredAddr);
            connectCommand += foundAddresses[i] + " -I";        
            command = connectCommand.c_str();
            cout << "command" << "\n";
            // system("coproc bluetoothctl");
            // system("echo -e 'info C9:0E:DB:EA:12:02\nexit' >&${COPROC[1]}");
            // system("output=$(cat <&${COPROC[0]})");
            // system("echo $output");
            system(command);
            //cout << "connect" << "\n";
            //system(connect);
        }
    }

    return 0;
    }
