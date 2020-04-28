#include "BLEService.h"


//map of all MAC addrs and their RSSI values in a scan
std::unordered_map<std::string, int> foundAddrs; 

//may of all MAC addrs and their RSSI found in the scan that are ready to be processed
std::unordered_map<std::string, int> approvedAddrs; 

//set of predefined MAC addrs valid for processing
std::unordered_set<std::string> desiredAddrs; 


//Is called within valFile.cpp main 
void initDesiredAddrs()
{
    desiredAddrs.insert("04:91:62:97:8B:37");
    desiredAddrs.insert("04:91:62:97:8B:38");
    desiredAddrs.insert("04:91:62:97:8B:39");
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
                   if(finalAddress.size() == MAC_ADDR_LEN)
                   {
                        foundAddrs[finalAddress] = 0;       //0 will be rssi
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
    bool beaconFound = false;
    std::unordered_map<std::string, int>:: iterator foundAddrsItr;
    std::unordered_set<std::string>:: iterator desiredAddrsItr;
    
    std::string cmnd = GetStdoutFromCommand("sudo timeout -s INT 0.25s hcitool lescan");
    
    parseToVector(cmnd);                                                             //adds addrs to foundAddresses

    for(foundAddrsItr = foundAddrs.begin(); foundAddrsItr != foundAddrs.end(); foundAddrsItr++) 
    {
        desiredAddrsItr = desiredAddrs.find(foundAddrsItr);
        if(desiredAddrsItr != desiredAddrs.end())                  //may need to change find logic
        {
            approvedAddrs[foundAddrsItr->first] = foundAddrsItr->second;
            beaconFound = true;
        }
    }

    //TODO: RSSI comparison on approved addrs need to have acurate rssi comparison
    //then could get the MAC addr with the lowest RSSI in string form
    //going to hard code in for now

    std::string nextAddr = "04:91:62:97:8B:38";

    if(beaconFound)     //or approved addr is not empty 
    {
        std::unordered_map<std::string, int>:: iterator approvedAddrsItr;
        approvedAddrsItr = approvedAddrs.find(nextAddr);

        return approvedAddrsItr->first;              
        //remove returned addr from approved addr
    }
    else
    {
        return NULL_STR;
    }   

    return 0;
}
