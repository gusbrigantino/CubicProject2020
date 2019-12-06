#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#define for_each_opt(opt, long, short)                                         \
  while ((opt = getopt_long(argc, argv, short ? short : "+", long, NULL)) != -1)

static void helper_arg(int min_num_arg, int max_num_arg, int *argc,
                       char ***argv, const char *usage) {
  *argc -= optind;
  /* too many arguments, but when "max_num_arg < min_num_arg" then no
           limiting (prefer "max_num_arg=-1" to gen infinity)
  */
  if ((*argc > max_num_arg) && (max_num_arg >= min_num_arg)) {
    fprintf(stderr, "%s: too many arguments (maximal: %i)\n", *argv[0],
            max_num_arg);
    printf("%s", usage);
    exit(1);
  }

  /* print usage */
  if (*argc < min_num_arg) {
    fprintf(stderr, "%s: too few arguments (minimal: %i)\n", *argv[0],
            min_num_arg);
    printf("%s", usage);
    exit(0);
  }

  *argv += optind;
}

static int find_conn(int s, int dev_id, long arg) {
  struct hci_conn_list_req *cl;
  struct hci_conn_info *ci;
  int i;

  if (!(cl = malloc(10 * sizeof(*ci) + sizeof(*cl)))) {
    perror("Can't allocate memory");
    exit(1);
  }
  cl->dev_id = dev_id;
  cl->conn_num = 10;
  ci = cl->conn_info;

  printf("Attempting ioctl");
  if (ioctl(s, HCIGETCONNLIST, (void *)cl)) {
    perror("Can't get connection list");
    printf("Error here");
    exit(1);
  }

  for (i = 0; i < cl->conn_num; i++, ci++)
    if (!bacmp((bdaddr_t *)arg, &ci->bdaddr)) {
      free(cl);
      return 1;
    }

  free(cl);
  return 0;
}

static struct option rssi_options[] = {{"help", 0, 0, 'h'}, {0, 0, 0, 0}};
static const char *rssi_help = "Usage:\n"
                               "\trssi <bdaddr>\n";
static void cmd_rssi(int dev_id, int argc, char **argv, char *bdad) {
  printf("\nIn the method\n");
  struct hci_conn_info_req *cr;
  bdaddr_t bdaddr;
  int8_t rssi;
  int opt, dd;
  /**
  for_each_opt(opt, rssi_options, NULL) {
  switch (opt) {
  default:
  printf("%s", rssi_help);
  return;
  }
  }
  //TODO:Fix this argc & argv method
  helper_arg(1, 1, &argc, &argv, rssi_help);
  str2ba(argv[0], &bdaddr);
  */
  str2ba(bdad, &bdaddr);
  // Problem might be here
  // Getting the bdaddr from command line instead of main method
  if (dev_id < 0) {
    dev_id = hci_for_each_dev(HCI_UP, find_conn, (long)&bdaddr);
    if (dev_id < 0) {
      fprintf(stderr, "Not connected.\n");
      exit(1);
    }
  }
  dd = hci_open_dev(dev_id);
  if (dd < 0) {
    perror("HCI device open failed");
    exit(1);
  }
  cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));
  if (!cr) {
    perror("Can't allocate memory");
    exit(1);
  }
  bacpy(&cr->bdaddr, &bdaddr);
  cr->type = ACL_LINK;
 if (ioctl(dd, HCIGETCONNINFO, (unsigned long)cr) < 0) {
    perror("Get connection info failed");
    exit(1);
  }
  if (hci_read_rssi(dd, htobs(cr->conn_info->handle), &rssi, 1000) < 0) {
    perror("Read RSSI failed");
    exit(1);
  }
  printf("RSSI return value: %d\n", rssi);
  free(cr);
  hci_close_dev(dd);
}

int main(int argc, char **argv) {

  inquiry_info *ii = NULL;
  int max_rsp, num_rsp;
  int dev_id, sock, len, flags;
  int i;
  int n;
  int8_t rssiVal;
  int ptype;
  char addr[19] = {0};
  char name[248] = {0};

  dev_id = hci_get_route(NULL);
  sock = hci_open_dev(dev_id);
  if (dev_id < 0 || sock < 0) {
    perror("opening socket");
    exit(1);
  }

  len = 8;
  max_rsp = 255;
  flags = IREQ_CACHE_FLUSH;
  ii = (inquiry_info *)malloc(max_rsp * sizeof(inquiry_info));

  num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
  if (num_rsp < 0)
    perror("hci_inquiry");

  for (i = 0; i < num_rsp; i++) {
    ba2str(&(ii + i)->bdaddr, addr);
    memset(name, 0, sizeof(name));
    if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0) <
        0)
      strcpy(name, "[unknown]");
    printf("%s %s\n", addr, name);
    printf("Testing rssi");
    cmd_rssi(dev_id, argc, argv, addr);
  }
  // printf("trying connection\n");
  // cmd_rssi(dev_id, argc, **argv);
  // printf("done");
  /*for (n = 0; n < num_rsp; n++) {
    uint16_t myhandle;
    ptype = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;
    if(hci_create_connection (sock, &(ii+i)->bdaddr, ptype,
    (ii+i)->clock_offset, 0, &myhandle, 0) < 0) perror("hci_create_connection");
    if(hci_read_rssi(sock, myhandle, &rssiVal, 0) < 0)
    printf("%i\n", rssiVal);
    hci_disconnect(sock, myhandle, HCI_OE_USER_ENDED_CONNECTION, 0);
    }*/
  free(ii);
  close(sock);
  return 0;
}
