//GUI - server
#include <iostream>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <time.h>

#define IP_SOCKET_PRTCL     0

#define PORT_NUMBER         8080

#define MAX_QUEUE_LEN       3           

#define MS_PER_SEC          1000

#define GUI_DELAY           200         //200ms

#define BUFFER_SIZE         1024        //2^11


int Timer(int milliseconds);



int main() 
{ 
	int guiSocket, new_socket;
	
    struct sockaddr_in address;
  
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[BUFFER_SIZE] = {0}; 
    
    
    if((guiSocket = socket(AF_INET, SOCK_STREAM, IP_SOCKET_PRTCL)) == 0)                                    // Creating socket file descriptor
    { 
        std::cout << "socket failed" << std::endl; 
        exit(EXIT_FAILURE); 
    } 

    
    if(setsockopt(guiSocket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))                                  //Modifying socket options
    { 
        std::cout << "socket options failed" << std::endl;  
        exit(EXIT_FAILURE); 
    } 
   
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT_NUMBER);

    
    if(bind(guiSocket, (struct sockaddr *)&address, sizeof(address)) < 0)                                   //Attaching socket to the port 8080 
    { 
        std::cout << "bind failed" << std::endl;  
        exit(EXIT_FAILURE); 
    } 

    while(true)
    {
        if(listen(guiSocket, MAX_QUEUE_LEN) < 0)                                                            //Listening for client
        { 
            std::cout << "listen failed" << std::endl;  
            exit(EXIT_FAILURE); 
        } 

        if((new_socket = accept(guiSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)         //accepts new client
        { 
            std::cout << "accept failed" << std::endl;  
            exit(EXIT_FAILURE); 
        } 

        recv(new_socket, buffer, BUFFER_SIZE, IP_SOCKET_PRTCL);                                          //recieves message from client 
        std::cout << buffer << std::endl; 
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