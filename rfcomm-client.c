#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#define true 1

int main(int argc, char **argv)
{
	struct sockaddr_rc addr = { 0};
	int s, status;
	char dest[18] = "DC:A6:32:02:C2:91";

	while(true){
	//allocate a socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	//set connection parameters 
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba(dest, &addr.rc_bdaddr );

	//connect to server
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	//send message
	while(status == 0) {
		status = write(s, "hello!", 6);
		sleep(1000);
	}

	if( status < 0 ) perror("uh oh");

	close(s);
	}
	return 0;
}
