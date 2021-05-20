#ifndef IF_ETHER_H
#define IF_ETHER_H


/*
 *	IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
 *	and FCS/CRC (frame check sequence).
 */

#define ETH_ALEN	6


/*
 *	These are the defined Ethernet Protocol ID's.
 */
//#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/

//Network byte order for now
#define ETH_P_ARP	0x0608

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	unsigned short		h_proto;		/* packet type ID field	*/
} __attribute__((packed));

#endif