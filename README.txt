Steps to run
    1. In one terminal window or in different device: $ python3 gui.py
        Change IP address and port number to match in valFile.cpp
        Can be modified to run on different device
    2. In different terminal window or different device: $ g++-9 -std=c++17 -Wall -pedantic -o validatorFile beacon.c valFile.cpp
        They must be compiled together or they will not link properly
    3. Run: $ ./validatorFile