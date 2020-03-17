#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int arg, char **argv)
{
	struct sockaddr_rc loc_addr = {0}, rem_addr = {0}, dest_addr = {0}, found_addr = {0};
	char buf[1024] = {0}; 
	int s, client, bytes_read;
	socklen_t opt = sizeof(rem_addr);
       	socklen_t found_opt = sizeof(found_addr);	
	//char beaconAddr[18] = "DC:A6:32:08:3D:E6";
	char beaconAddr[18] = "04:91:62:97:97:23";
	str2ba(beaconAddr, &dest_addr.rc_bdaddr);
	char attemptedAddr[19] = {0};
	char destAddr[1024] = {0};

	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	int i;

	bool isConnected = false;

	dev_id = hci_get_route(NULL);
	sock = hci_open_dev( dev_id);
	if (dev_id <0 || sock < 0){
		perror("opening socket");
		exit(1);
	}
	

	//allocate socket 
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	
	//bind socket to port 1 of the first available local bluetooth adapter 
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;
	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
	
	//start the struct for found bluetooth devices
	found_addr.rc_family = AF_BLUETOOTH;
	found_addr.rc_channel = (uint8_t) 1;
	
	while(!isConnected){

	//put socket into listening mode
	listen(s,1);
	
	len = 8;
	max_rsp = 255;
	flags = IREQ_CACHE_FLUSH;
	ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	if(num_rsp < 0) perror("hci_inquiry");

	for(i = 0; i < num_rsp; i++) {
		//changed addr to attemptedAddr
		ba2str(&(ii+i)->bdaddr, attemptedAddr);
		str2ba(attemptedAddr, &found_addr.rc_bdaddr);

	

	//accept one connection 
	//client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	
	//eventually will need array of Bluetooth structs
	//str2ba(beaconAddr, beaconBD);
	
	//ba2str(&rem_addr.rc_bdaddr, attemptedAddr);
	//redundant for clarity
	//ba2str(&dest_addr.rc_bdaddr, destAddr);
	
		printf("Attempted: %s\nDesired: %s\n", attemptedAddr, beaconAddr);
	if(strcmp(attemptedAddr, beaconAddr) == 0){
		
		isConnected = true;
		//changed accept to connect
		//client = accept(s, (struct sockaddr *)&found_addr, &found_opt);
		client = connect(s, (struct sockaddr *)&found_addr, sizeof(found_addr));
		//ba2str( &rem_addr.rc_bdaddr, buf );
		fprintf(stderr, "accepted connection from %s\n", attemptedAddr);
		memset(attemptedAddr, 0, sizeof(attemptedAddr));
	
	//read data from the client 
	bytes_read = read(client, buf, sizeof(buf));
	if( bytes_read > 0)
	{
		printf("received [%s]\n", buf); 
	}
	
	}
	else{
		if(strlen(attemptedAddr) > 0){
		printf("wrong address: %s", attemptedAddr);
		isConnected = false;
		}
		else{
		printf("Something went wrong");
		}
	}
	}
	}
	free( ii );
	close(sock);
	isConnected = false;
	//close connection 
	close(client); 
	close(s); 
	return(0); 
}

	
