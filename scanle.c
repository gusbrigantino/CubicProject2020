#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


int main(int argc, int** argv) {

	int hciSocket;
	struct hci_filter newFilter, originalFilter;

	hciSocket = hci_open_dev(0);

	//save original filter
	socklen_t originalFilterLen = sizeof(originalFilter);
	getsockopt(hciSocket, SOL_HCI, HCI_FILTER, &originalFilter, &originalFilterLen);

	//setup new filter
	hci_filter_clear(&newFilter);
	hci_filter_set_ptype(HCI_EVENT_PKT, &newFilter);
	hci_filter_set_event(EVT_LE_META_EVENT, &newFilter);
	setsockopt(hciSocket, SOL_HCI, HCI_FILTER, &newFilter, sizeof(newFilter));

	//disable scanning
	hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);

	//set scan params
	hci_le_set_scan_parameters(hciSocket, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0, 1000);

	//start scanning
	hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);
	hci_le_set_scan_enable(hciSocket, 0x01, 0, 1000);

	printf("Scanning ...\n");

	unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr;
	int len;

	evt_le_meta_event *meta;
	le_advertising_info *info;
	char addr[18];

	fd_set rfds;
	struct timeval tv;
	int selectRetval;

	FD_ZERO(&rfds);
	FD_SET(hciSocket, &rfds);

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	selectRetval = select(hciSocket + 1, &rfds, NULL, NULL, &tv);
	//printf("selectRetval: %d\n", selectRetval);
	if(-1 == selectRetval) {
		printf("error");
	}
	printf("stuck here");
	len = read(hciSocket, buf, sizeof(buf));
	printf("len: %d\n", len);
	if(len == 0) {
		printf(" ");
	}

	ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
	len -= (1 + HCI_EVENT_HDR_SIZE);

	meta = (evt_le_meta_event *) ptr;
	printf("HERE");
	if (meta->subevent != 0x02)
		printf("error");

	info = (le_advertising_info *) (meta->data + 1);
	ba2str(&info->bdaddr, addr);
	printf("Found: %s\n", addr);


	return 0;
}
