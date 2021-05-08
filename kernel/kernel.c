#include <stdint.h>
#include "pci.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "util.h"
#include "rtl8139.h"


int main() {
	
	clear_screen();
	kprint("initializing IDT: idt_init()\n");
	idt_init();
	kprint("initiatilizing PCI: pci_init()\n");
	pci_init();
    //init_timer(100);
	rtl8139_init();

	 for(;;);
   return 0;
}