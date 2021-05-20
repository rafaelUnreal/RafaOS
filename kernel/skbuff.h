#ifndef SKBUFF_H
#define SKBUFF_H

#include "arp.h"

struct sk_buff {

	union {
		//struct tcphdr	*th;
		//struct udphdr	*uh;
		//struct icmphdr	*icmph;
		//struct igmphdr	*igmph;
		//struct iphdr	*ipiph;
		//struct ipv6hdr	*ipv6h;
		unsigned char	*raw;
	} h;
	
	
	//struct net_device	*dev;
	//struct net_device	*input_dev;
	//struct net_device	*real_dev;
	
	union {
		//struct iphdr	*iph;
		//struct ipv6hdr	*ipv6h;
		struct arphdr	*arph;
		unsigned char	*raw;
	} nh;

	union {
		unsigned char	*raw;
	} mac;

	//struct  dst_entry	*dst;
	//struct	sec_path	*sp;
	char			cb[40];
	unsigned int		len, data_len, mac_len;
	unsigned char		*head,*data,*tail,*end;


};



#endif