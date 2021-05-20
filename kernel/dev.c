#include "dev.h"
#include "list.h"

struct list_head ptype_base[PTYPE_HASH_SIZE];

#define PTYPE_HASH_MASK 15

static inline struct list_head *ptype_head(const struct packet_type *pt)
{
	//return &ptype_base[pt->type & PTYPE_HASH_MASK];
		return &ptype_base[0];
}


void dev_add_pack(struct packet_type *pt)
{
	struct list_head *head = ptype_head(pt);
	int hash;
	
	//hash = pt->type & 15;
	//spin_lock(&ptype_lock);
	list_add(&pt->list, head);
	//spin_unlock(&ptype_lock);
}

int net_dev_init(void){
	
	int i;
	for (i = 0; i < PTYPE_HASH_SIZE; i++){
		INIT_LIST_HEAD(&ptype_base[i]);
	}
	
	
}