#include "beacon.h"

static int BeaconFuncCallCount = 0;

int BeaconSimulator(int randWaitTime)
{
    
    if(BeaconFuncCallCount++ == randWaitTime)               //BeaconFuncCallCount is a global static var used to count how many times 
                                                            //the state machine iterates through IDLE_ST to time the process
    {
        int randAcctNum = rand() % ACCT_NUM_MAX;

        BeaconFuncCallCount = 0;
        return randAcctNum;
    }
    else
    {
        return -1;
    }

}