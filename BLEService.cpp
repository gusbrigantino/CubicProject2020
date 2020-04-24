#include "BLEService.h"


std::vector<std::string> foundAddresses;
std::vector<std::string> approvedAddr;

void BLEReset()
{
    system("sudo hciconfig hci0 down");
    system("sudo hciconfig hci0 up");
}

//parses the terminal output by newline char first
//then parses those lines by space to separate the name and addr
void parseToVector(std::string terminalOutput) 
{
    std::stringstream ss(terminalOutput);
    std::string to;

    if(!terminalOutput.empty())
    {
        while(getline(ss,to,'\n'))
        {
           std::stringstream to_ss(to);
           std:;string finalAddress;

           if(!to.empty())
           {
               while(getline(to_ss,finalAddress,' '))
               {
                   if(finalAddress.size() == 17)
                   {
                        foundAddresses.push_back(finalAddress);
                   }
               }
           }
        }
    }
}

string GetStdoutFromCommand(string cmd) 
{

    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    BLEReset();

    stream = popen(cmd.c_str(), "r");

    if(stream) 
    {
        while (!feof(stream))
        {
            
        }
        if (fgets(buffer, max_buffer, stream) != NULL) 
        {
            data.append(buffer);
        }
        pclose(stream);
    }
    return data;
}

//TODO: Replace string with vector<string> of all accetable addr
std::string desiredAddr = "04:91:62:97:8B:38";

std::string getBLEAddr()
{
    //if(approvedAddr.size() > 0)
    return approvedAddr[0];

}

std::string BLEService()
{
    std::string connectCommand = "sudo gatttool -b ";
    //string connectCommand = "connect ";
    //const char *command;
    //const char *connect = "connect";

    std::string cmnd = GetStdoutFromCommand("sudo timeout -s INT 5s hcitool lescan");
    
    parseToVector(cmnd);

    for(int i = 0; i < (int)foundAddresses.size(); i++)
    {
        if(desiredAddr.compare(foundAddresses[i]) == 0)
        {
            approvedAddr.push_back(desiredAddr);
            //connectCommand += foundAddresses[i] + " -I";        
            //command = connectCommand.c_str();
            //cout << "command" << "\n";
            // system("coproc bluetoothctl");
            // system("echo -e 'info C9:0E:DB:EA:12:02\nexit' >&${COPROC[1]}");
            // system("output=$(cat <&${COPROC[0]})");
            // system("echo $output");
            //system(command);
            //cout << "connect" << "\n";
            //system(connect);
        }
    }

    return approvedAddr[0];
}
