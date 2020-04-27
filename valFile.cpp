/*
    Gus Brigantino
    Validator Simulator Module
    Senior Design Spring 2020
    Cubic Transportation
*/

#include "valFile.h"

Acct Account;
std::unordered_map<std::string, int> recentlyProcessedAddrs;

int main()
{   
    initDesiredAddrs();                                                 //From BLEService

    ValidationProcess();
}


int ValidationProcess()
{           
    std::string beaconAcctNum;                                          //account number from beacon holder

    int machineState = BLE_ST;                                          //init state var of the state machine

    while(true)
    {                                    
        switch(machineState)
        {
            case BLE_ST:                                                //simulates waiting for a beacon to arrive 

                beaconAcctNum = BLEService();

                if(beaconAcctNum.compare("\0") == 0)                    //no beacon found
                {
                    machineState = BLE_ST;
                }
                else
                {
                    std::cout << beaconAcctNum << std::endl;
                    if(recentlyProcessedAddrs.find(beaconAcctNum) == recentlyProcessedAddrs.end()) 
                    {
                        //Approved
                        recentlyProcessedAddrs.insert(std::make_pair(beaconAcctNum, 0));

                        Account.setNumber(beaconAcctNum);               //random account number 0-10 TODO: MAC Addr
                        UpdateRecentlyProcessedAddrs();
                        machineState = LOOKUP_ST;   
                    }
                    else
                    {
                        //processed within recent time limit
                        UpdateRecentlyProcessedAddrs();
                        machineState = BLE_ST;
                    }
                }
                break;

            case LOOKUP_ST:                                             //finds given account number from beacon in csv file   

                AccountLookUp();

                if(Account.getFoundStatus() && Account.getBalanceStatus())              
                {
                    machineState = DB_EDIT_ST;
                }
                else
                {
                    machineState = UI_ST;
                }
                break;

            case DB_EDIT_ST:                                            //makes edits to exsisting csv file by creating new one and renaming

                UpdateDataBase();
                
                machineState = UI_ST;
                break;

            case UI_ST:                                                 //simple ui to output line for now     

                PrintUserInterface();

                machineState = BLE_ST;
                break;
        }
        Timer(ST_MACH_DELAY);                                           //clocks the state machine to run on an interval
    }
    return 0;
}




int AccountLookUp()
{
    std::fstream dataFile;                                      //fstream = file read/write default

    std::string line;                                           //std string used in get line calls for reading the csv file

    std::string dbAcctNum;                                      //account number from database holder

    std::string acctName;                                       //account name from database holder

    double acctBalance;
    std::string balance;                                        //vars used to convert balance from string to int and save 

    int acctIndex = 0;                                          //account index in csv file tracker

    dataFile.open(DB_FILE_NAME, std::ios::in);                  //open csv file

    if(dataFile.is_open())
    {                      
        while(getline(dataFile, line, DELIMITER))
        {   
            std::stringstream acctNumHolder(line);              //determines if the account number 
            acctNumHolder >> dbAcctNum;                         //Assuming the number is contained in the first column of every row

            if(dbAcctNum == Account.getNumber())                //if last account number read equals account number from beacon
            {
                getline(dataFile, acctName, DELIMITER);                
                Account.setName(acctName);                      //save account name

                getline(dataFile, balance, DELIMITER);
                std::stringstream balanceHolder(balance);
                balanceHolder >> acctBalance;                   
                Account.setBalance(acctBalance);                //convert balance to an int and save

                if(Account.getBalance() >= TICKET_PRICE)        //Balance check so that the account does not go below $0
                {
                    Account.setBalanceStatus(true);
                }
                else
                {
                    Account.setBalanceStatus(false);
                }
                
                Account.setFoundStatus(true);                   //set found status to true
            }
            else                                                
            {
                getline(dataFile, line);                        //reads to end of line to allow next read to be at the beginning of the next line

                Account.setFoundStatus(false);                  //set found status to false
            }

            Account.setIndex(acctIndex);                        //update index for object
            acctIndex++;                                        //since account number not found yet inc

            if(Account.getFoundStatus())                        //if account number is found break out of loop to avoid unnecessary iterations
            {
                break;
            }
        }
        dataFile.close();
    }
    else
    {
        std::cout << "File could not be opened." << std::endl;
    }

    return 0;
}




int UpdateDataBase()
{
    std::fstream fileRead;                                          //file to read from
    std::fstream fileWrite;                                         //file to write to 

    std::string line2;                                              //std string used in get line calls for reading the csv file
    
    std::vector<std::string> row;                                   //vector of strings used to hold entire rows (lines) of csv file
    std::string newRow;                                             //string used to hold the updated row 
    int csvRowCnt;                                                  //count the total number of rows in the csv file
    
    fileRead.open(DB_FILE_NAME, std::ios::in);                      //open existing database csv file
    fileWrite.open(NEW_DB_FILE_NAME, std::ios::out);                //create new csv file

    if(fileRead.is_open() && fileWrite.is_open())                   //if both are opened 
    {
        csvRowCnt = 0;
        
        while(getline(fileRead, line2))
        {
            row.push_back(line2);                                   //store each line in the vector of strings
            csvRowCnt++;
        }

        Account.setBalance(Account.getBalance() - TICKET_PRICE);    //update balance of account

        newRow = Account.getNumber() + DELIMITER 
            + Account.getName() + DELIMITER 
            + std::to_string(Account.getBalance());                 //creates a new row based on current Acct Account info 

        row[Account.getIndex()] = newRow;                           //saves new row in correct index

        for(int k = 0; k < csvRowCnt; k++)
        {
            fileWrite << row[k] << '\n';                            //writes the entire database to the new csv file
        }
        
        fileWrite.close();
        fileRead.close();
        remove(DB_FILE_NAME);                                       //removes the old file from directory
        rename(NEW_DB_FILE_NAME, DB_FILE_NAME);                     //renames the new file to the original name
    }
    else
    {
        std::cout << "File could not be open." << std::endl;
    }

    return 0;
}




int PrintUserInterface()
{
    //if the account number given by the beacon is not in the csv file
    if(!Account.getFoundStatus())
    {
        std::cout << "Your account could not be found." << std::endl << std::endl;
    }
    //if the account number given by the beacon is in the csv file but lacks the funds to purchase a ticket
    else if(!Account.getBalanceStatus())
    {
        std::cout << "Welcome " << Account.getName() << std::endl;
        std::cout << "You do not have enough funds to purchase a ticket." << std::endl;
        std::cout << "Your current balance is $" << Account.getBalance() << std::endl;
        std::cout << "You need $" << (TICKET_PRICE - Account.getBalance()) << " more to purchase a ticket" << std::endl << std::endl;
    }
    //if the account number given by the beacon is found and has the funds to purchase a ticket 
    else
    {
        std::cout << "Welcome " << Account.getName() << std::endl;
        std::cout << "Your new balance is $" << Account.getBalance() << std::endl;
        std::cout << "Enjoy your trip!" << std::endl << std::endl;
    }
    

    return 0;
}


int UpdateRecentlyProcessedAddrs()
{
    std::unordered_map<std::string, int>:: iterator acctInfoItr;
    
    for(acctInfoItr = recentlyProcessedAddrs.begin(); acctInfoItr != recentlyProcessedAddrs.end(); acctInfoItr++) 
    { 
        acctInfoItr->second = (acctInfoItr->second + 1);

        if(acctInfoItr->second == 25)
        {
            recentlyProcessedAddrs.erase(acctInfoItr);
        }
    }

    return 0;
}



int Timer(int milliseconds)
{
    clock_t startTime = clock(); //Start timer

    double millisecondsPassed;
    double millisecondsToDelay = milliseconds;

    bool timerFlag = true;
    while(timerFlag)
    {
        millisecondsPassed = (clock() - startTime) / MS_PER_SEC;
        if(millisecondsPassed >= millisecondsToDelay)
        {
            timerFlag = false;
        }
    }
    return 0;
}




//Acct Class Functions
Acct::Acct()
{
    name = "";
    number = "";
    balance = 0.0;
    index = 0;
    foundStatus = false;
    balanceStatus = false;
}


std::string Acct::getName()
{
    return name;
}

void Acct::setName(std::string newName)
{
    name = newName;
}

std::string Acct::getNumber()
{
    return number;
}

void Acct::setNumber(std::string newNumber)
{
    number = newNumber;
}

double Acct::getBalance()
{
    return balance;
}

void Acct::setBalance(double newBalance)
{
    balance = newBalance;
}

int Acct::getIndex()
{
    return index;
}

void Acct::setIndex(int newIndex)
{
    index = newIndex;
}

bool Acct::getFoundStatus()
{
    return foundStatus;
}

void Acct::setFoundStatus(bool newFoundStatus)
{
    foundStatus = newFoundStatus;
}

bool Acct::getBalanceStatus()
{
    return balanceStatus;
}
    
void Acct::setBalanceStatus(bool newBalanceStatus)
{
    balanceStatus = newBalanceStatus;
}