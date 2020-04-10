#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <chrono>
#include <signal.h>

using namespace std;

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

struct BLE{
	int hciSocket;
  struct hci_filter newFilter, originalFilter;
  //map<string, time_t> found;

  void open() {
    hciSocket = hci_open_dev(0);

    // save original filter
    socklen_t originalFilterLen = sizeof(originalFilter);
    getsockopt(hciSocket, SOL_HCI, HCI_FILTER, &originalFilter, &originalFilterLen);

    // setup new filter
    hci_filter_clear(&newFilter);
    hci_filter_set_ptype(HCI_EVENT_PKT, &newFilter);
    hci_filter_set_event(EVT_LE_META_EVENT, &newFilter);
    setsockopt(hciSocket, SOL_HCI, HCI_FILTER, &newFilter, sizeof(newFilter));

    // disable scanning just in case scanning was already happening,
    // otherwise hci_le_set_scan_parameters call will fail
    hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);

    // set scan params
    hci_le_set_scan_parameters(hciSocket, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0, 1000);

    // start scanning
    hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);
    hci_le_set_scan_enable(hciSocket, 0x01, 0, 1000);

    printf("test4\n");
    printf("Scanning ...\n");
    printf("test3\n"); 
   
}
    void close() {
    // put back original filter
    setsockopt(hciSocket, SOL_HCI, HCI_FILTER, &originalFilter, sizeof(originalFilter));

    // stop scanning
    hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);

    // close BLE adaptor
    hci_close_dev(hciSocket);
  }
  string get_device()
  {
    printf("test6\n");
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
    printf("test8\n");
    // wait to see if we have data on the socket
    selectRetval = select(hciSocket + 1, &rfds, NULL, NULL, &tv);
    printf("test9\n");
    if (-1 == selectRetval) {
      return "error";
    }
    printf("hciSocket %d\n", hciSocket);
    printf("size of buf %d\n", sizeof(buf));
    printf("buf %d\n", buf);
    len = read(hciSocket, buf, sizeof(buf));
    //len = hciSocket.read(buf, sizeof(buf));
    printf("len %d\n", len);
    printf("test10\n");
    if (len == 0) {
    	return "";
    }

    ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
    len -= (1 + HCI_EVENT_HDR_SIZE);

    meta = (evt_le_meta_event *) ptr;

    if (meta->subevent != 0x02)
      return "error";

    info = (le_advertising_info *) (meta->data + 1);
    ba2str(&info->bdaddr, addr);
    //discovered(addr);
    printf("test7\n");
    printf("%s\n", addr);
    return addr;
  }
};

BLE ble; 

int main(){
    string address;
    ble.open();
    
    printf("Scanning...\n");
    printf("test2\n");
    
    for(;;){
        printf("test\n"); 
        address = ble.get_device();
        printf("%s test5 \n", address);
    }
    return 0;
}
