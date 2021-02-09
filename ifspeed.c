//#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#define ETHTOOL_GSET		0x00000001 /* DEPRECATED, Get settings.
					    * Please use ETHTOOL_GLINKSETTINGS
					    */

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;

  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s == -1)
    return 2;

  struct ethtool_cmd cmd = {.cmd = ETHTOOL_GSET};
  struct ifreq req;
  strncpy(req.ifr_ifrn.ifrn_name, argv[1], IF_NAMESIZE);
  req.ifr_ifrn.ifrn_name[IF_NAMESIZE - 1] = 0;
  req.ifr_ifru.ifru_data = (void *)&cmd;

  if (ioctl(s, SIOCETHTOOL, &req) == -1)
    return 3;

  printf("speed=%d duplex=%s\n", ethtool_cmd_speed(&cmd),
         cmd.duplex == DUPLEX_FULL   ? "full"
         : cmd.duplex == DUPLEX_HALF ? "half"
                                     : "unknown");

  return 0;
}
