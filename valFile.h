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
#include <cstring>
#include <sys/socket.h> 
#include <arpa/inet.h> 

#include "beacon.h"

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

#define IP_SOCKET_PRTCL     0

#define PORT_NUMBER         8080

#define SERVER_ADDR         "127.0.0.1"         //"192.168.1.31"        //change this to desire IP address of server

#define BUFFER_SIZE         1024


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
This function handles the UI client that feeds data to socket for GUI
*/
int UIClient();
/*
This function is a simple software timer used to clock the state machine
*/
int Timer(int milliseconds);


class Acct
{
    private:
    std::string name;                               //name of account holder
    int number;                                     //account number
    double balance;                                 //current account balance
    int index;                                      //where account is located in csv file based on rows
    bool foundStatus;                               //if the account was sucessfully found in the database
    bool balanceStatus;                             //if the current balance contains enough to purcahse a ticket

    public:

    Acct();

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