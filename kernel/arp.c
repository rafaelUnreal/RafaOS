#include "arp.h"
#include "dev.h"
#include "if_ether.h"
#include "../drivers/screen.h"



static int arp_rcv(struct sk_buff *skb, unsigned char *dev,
		   struct packet_type *pt)
{
	const struct arphdr *arp;
	kprint("arp_rcv()");

}

static struct packet_type arp_packet_type = {
	.type =	ETH_P_ARP,
	.func =	arp_rcv,
};

void arp_init(void)
{
	//neigh_table_init(NEIGH_ARP_TABLE, &arp_tbl);
	dev_add_pack(&arp_packet_type);
	//arp_proc_init();
	//register_netdevice_notifier(&arp_netdev_notifier);
}