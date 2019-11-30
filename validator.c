#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    int n;
    int8_t rssiVal;
    int ptype;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), 
            name, 0) < 0)
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }
    for (n = 0; n < num_rsp; n++) {
	 uint16_t myhandle;
	 ptype = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;
	 if(hci_create_connection (sock,
				 &(ii+i)->bdaddr,
				 ptype,
				 (ii+i)->clock_offset,
				 0,
				 &myhandle,
				 0) < 0) perror("hci_create_con");
	 if(hci_read_rssi(sock, myhandle, rssiVal, 0) < 0)
		 rssiVal = -127;
	 hci_disconnect(sock, myhandle, HCI_OE_USER_ENDED_CONNECTION, 0);
    }
    free( ii );
    close( sock );
    return 0;
}
