#include "BLEService.h"


std::vector<std::string> foundAddrs;
std::vector<std::string> approvedAddrs;
std::unordered_map<std::string, int> desiredAddrs;


//TODO: change storage system to unordered map
//TODO: figure out what to do with processed beacons still in range 
//Will replace desiredAddrs vector map to either a 1 or a 0 depending on if recently processed?? 


void initDesiredAddrs()
{
    desiredAddrs["04:91:62:97:8B:37"] = 0;
    desiredAddrs["04:91:62:97:8B:38"] = 0;
    desiredAddrs["04:91:62:97:8B:39"] = 0;
    //more addresses can be added here
}


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
           std::string finalAddress;

           if(!to.empty())
           {
               while(getline(to_ss,finalAddress,' '))
               {
                   if(finalAddress.size() == 17)
                   {
                        foundAddrs.push_back(finalAddress);
                   }
               }
           }
        }
    }
}


std::string GetStdoutFromCommand(std::string cmd) 
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
        while(!feof(stream))
        {
            if(fgets(buffer, max_buffer, stream) != NULL) 
            {
                data.append(buffer);
            }
	}
        pclose(stream);
    }
    return data;
}


std::string BLEService()
{
    initDesiredAddrs();

    bool beaconFound = false;
    std::string cmnd = GetStdoutFromCommand("sudo timeout -s INT 0.25s hcitool lescan");
    
    parseToVector(cmnd);            //adds addrs to foundAddresses

    for(size_t i = 0; i < foundAddrs.size(); i++)
    {
        if (desiredAddrs.find(foundAddrs[i]) != desiredAddrs.end()) 
        {
            //Not sure what to do here may switch to !=
            //cout << key << " not found\n\n"; 
            approvedAddrs.push_back(foundAddrs[i]);
            beaconFound = true;
        }
        else
        {
            
        }
    }

    //TODO: RSSI comparison on approved addrs

    if(beaconFound)
    {
        return approvedAddrs[0];
    }
    else
    {
        return "\0";
    }   

    return 0;
}
