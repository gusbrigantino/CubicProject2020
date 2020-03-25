#include "beaconSim.h"
static int BeaconFuncCallCount = 0;

int main()
{
    printf("test\n");

    int test = BeaconSimulator(0);

    printf(test + "\n");
}

int BeaconSimulator(int randWaitTime)
{
    printf("test2\n");
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
