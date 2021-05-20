#include <stdint.h>
#include "pci.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "util.h"
#include "rtl8139.h"

//Networking subsystem
#include "dev.h"
#include "arp.h"


int main() {
	
	clear_screen();
	kprint("initializing IDT: idt_init()\n");
	idt_init();
	kprint("initiatilizing PCI: pci_init()\n");
	pci_init();
	kprint("initiatilizing NetDev: net_dev_init()\n");
	net_dev_init();
	
    //init_timer(100);
	kprint("initiatilizing RTL8139 rtl8139_init()\n");
	rtl8139_init();
	kprint("initiatilizing ARP: arp_init()");
	arp_init();

	 for(;;);
   return 0;
}