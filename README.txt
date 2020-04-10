valFile.cpp and gui.py

Run GUI and Validator on same machine
1. In gui.py change IPAddress to '127.0.0.1'
2. In valFile.h change SERVER_ADDR to 127.0.0.1
3. Make sure PORT_NUMBER in valFile.h and portNumber in gui.py are the same 
4. Open two terminal windows
5. In one terminal window run the GUI: 
    $ python3 gui.py
6. In the other terminal window compile valFile.cpp AND beacon.c (they must be compiled together or will not link):
    $ g++-9 -std=c++17 -Wall -pedantic -o validatorFile beacon.c valFile.cpp
7. Run the executable:
    $ ./validatorFile


Run GUI and Validator on different machines
1. In gui.py change IPAddress to the ip address of the GUI (server)
    -To get IP address of a Raspberry Pi on same network:
        $ ping raspberrypi.local on laptop
2. In valFile.h change SERVER_ADDR to server IP address
3. Make sure PORT_NUMBER in valFile.h and portNumber in gui.py are the same 
4. On one machine run the GUI: 
    $ python3 gui.py
6. On the other machine compile valFile.cpp AND beacon.c (they must be compiled together or will not link):
    $ g++-9 -std=c++17 -Wall -pedantic -o validatorFile beacon.c valFile.cpp
7. Run the executable:
    $ ./validatorFile