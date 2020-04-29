#include "BLEService.h"


//map of all MAC addrs and their RSSI values in a scan
std::unordered_map<std::string, int> foundAddrs; 

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


void ParseBtmgmtFind(std::string terminalOutput) 
{
    std::stringstream lineStream(terminalOutput);
    std::string lineHolder;

    std::string devAddr;
    std::string devRssi;

    if(!terminalOutput.empty())
    {
        while(getline(lineStream, lineHolder))
        {
            std::stringstream tokenStream(lineHolder);
            std::string tokenHolder;
            std::vector<std::string> line;

            while(getline(tokenStream, tokenHolder, ' '))     //would like to break if [1] not "dev_found" instead of tokenizing whole string no matter what
            {
                line.push_back(tokenHolder);
            }
            
            if(line[1].compare("dev_found:") == 0)       //if equal meaning correct line with the info
            {
                devAddr = line[2];
                devRssi = line[7];

                devRssi = devRssi.erase(0, 1);
                int rssi = stoi(devRssi);

                foundAddrs[devAddr] = rssi;
            }
        }
    }
}


std::string BLEService()
{
    //may of all MAC addrs and their RSSI found in the scan that are ready to be processed
    std::unordered_map<std::string, int> approvedAddrs; 

    bool beaconFound = false;

    //iterators
    std::unordered_map<std::string, int>:: iterator foundAddrsItr;
    std::unordered_set<std::string>:: iterator desiredAddrsItr;
    
    std::string cmnd = GetStdoutFromCommand(BTMGMT_FIND);
    
    ParseBtmgmtFind(cmnd);

    for(foundAddrsItr = foundAddrs.begin(); foundAddrsItr != foundAddrs.end(); foundAddrsItr++)         //iterates through all of the addrs found in the scan
    {
        desiredAddrsItr = desiredAddrs.find(foundAddrsItr->first);                                      //searches for found addr in the set of predefined addrs for a match
        if(desiredAddrsItr != desiredAddrs.end())                  
        {
            approvedAddrs[foundAddrsItr->first] = foundAddrsItr->second;                                //adds matches and their rssi value to map of approved addrs
            beaconFound = true;
        }
    }

    //TODO: RSSI comparison on approved addrs need to have acurate rssi comparison
    //needs to somehow update rssi every iteration 
        //either update existing entry or delete all at end of func and start over
        //deleting all seems to be the simplest
    //then could get the MAC addr with the lowest RSSI in string form
    //going to hard code in for now

    std::string nextAddr = "04:91:62:97:8B:38";

    if(beaconFound)     //or approved addr is not empty 
    {
        approvedAddrs.erase(nextAddr);                                                                  //removes by key

        return nextAddr;              
    }
    else
    {
        return NULL_STR;
    }   

    return 0;
}
