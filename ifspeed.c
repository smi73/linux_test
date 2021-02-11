//#include <linux/ethtool.h>
#include <linux/types.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

/* This should work for both 32 and 64 bit userland. */
struct ethtool_cmd {
	__u32	cmd;
	__u32	supported;	/* Features this interface supports */
	__u32	advertising;	/* Features this interface advertises */
	__u16	speed;		/* The forced speed, 10Mb, 100Mb, gigabit */
	__u8	duplex;		/* Duplex, half or full */
	__u8	port;		/* Which connector port */
	__u8	phy_address;
	__u8	transceiver;	/* Which transceiver to use */
	__u8	autoneg;	/* Enable or disable autonegotiation */
	__u32	maxtxpkt;	/* Tx pkts before generating tx int */
	__u32	maxrxpkt;	/* Rx pkts before generating rx int */
	__u16	speed_hi;
	__u16	reserved2;
	__u32	reserved[3];
};

#define ETHTOOL_GSET		0x00000001 /* DEPRECATED, Get settings.
					    * Please use ETHTOOL_GLINKSETTINGS
					    */
#define ETHTOOL_GLINKSETTINGS	0x0000004c /* Get ethtool_link_settings */

/* Duplex, half or full. */
#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01

static inline __u32 ethtool_cmd_speed(struct ethtool_cmd *ep)
{
	return (ep->speed_hi << 16) | ep->speed;
}

int main(int argc, char *argv[]) {
  printf("Testing...\n");
  if (argc != 2)
    return 1;

  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s == -1)
    return 2;

  struct ethtool_cmd cmd = {.cmd = ETHTOOL_GLINKSETTINGS};
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
