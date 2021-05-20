#ifndef DEV_H
#define DEV_H

#include "skbuff.h"
#include "list.h"

#define PTYPE_HASH_SIZE 16

struct packet_type
{
 unsigned short type;
 //struct net_device *dev;
 int (*func) (struct sk_buff *, unsigned char *, struct packet_type *);
 void *af_packet_priv;
 struct list_head *list;
};

void dev_add_pack(struct packet_type *pt);
int net_dev_init(void);

#endif