/*
    Gus Brigantino
    Validator Simulator Module Header
    Senior Design Spring 2020
    Cubic Transportation
*/

#include <iostream>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>

#define IDLE_ST             0
#define LOOKUP_ST           1
#define DB_EDIT_ST          2
#define UI_ST               3

#define ST_MACH_DELAY       200                 //200ms

#define MS_PER_SEC          1000

#define BEACON_WAIT_TIME    40                  //state machine clocks every 200ms 40*200 = 8000ms
                                                //max beacon wait time is equivalent to 8 seconds

#define ACCT_NUM_MAX        10                  //assumes account numbers begin at 0

#define DB_FILE_NAME        "AcctDB.csv"
#define NEW_DB_FILE_NAME    "newAcctDB.csv"

#define TICKET_PRICE        2.5                 //$2.50 for a ticket

#define DELIMITER           ','                 //used for reading and writing lines from and to csv files


/*
This function contains the state machine that simulates the validator
*/
int ValidationProcess();
/*
This function takes an account number from the beacon (simulated) and determines if the account 
is valid using the databse (csv file)
*/
int AccountLookUp();
/*
This function updates the balance of a charged account in the database
*/
int UpdateDataBase();
/*
This function is responsible for user interface
TODO: This is where GUI code will go
*/
int PrintUserInterface();
/*
This function simulates a beacon by randomizing a delay and account number 
Will not be needed in final project
*/
int BeaconSimulator(int randWaitTime);
/*
This function is a simple software timer used to clock the state machine
*/
int Timer(int milliseconds);


class Acct
{
    private:
    std::string name = "";                                  //name of account holder
    int number = 0;                                         //account number
    double balance = 0.0;                                   //current account balance
    int index = 0;                                          //where account is located in csv file based on rows
    bool foundStatus = false;                               //if the account was sucessfully found in the database
    bool balanceStatus = false;                             //if the current balance contains enough to purcahse a ticket

    public:
    std::string getName();
    void setName(std::string newName);
    int getNumber();
    void setNumber(int newNumber);
    double getBalance();
    void setBalance(double newBalance);
    int getIndex();
    void setIndex(int newIndex);
    bool getFoundStatus();
    void setFoundStatus(bool newFoundStatus);
    bool getBalanceStatus();
    void setBalanceStatus(bool newBalanceStatus);
};                                                          //Account class used to store patron info from database and beacon