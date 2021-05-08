#include "rtl8139.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "util.h"

uint32_t current_packet_ptr;


void rtl8139_init(){
	
	TSAD_array[0] = 0x20;
	TSAD_array[1] = 0x24;
	TSAD_array[2] = 0x28;
	TSAD_array[3] = 0x2C;
	
	TSD_array[0] = 0x10;
	TSD_array[1] = 0x14;
	TSD_array[2] = 0x18;
	TSD_array[3] = 0x1C;
	
	
	kprint("pci_get_device()\n");
	pci_rtl8139_device = pci_get_device(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID, -1);
    uint32_t ret = pci_read(pci_rtl8139_device, PCI_BAR0);
    rtl8139_device.bar_type = ret & 0x1;
	
    // Get io base or mem base by extracting the high 28/30 bits
    rtl8139_device.io_base = ret & (~0x3);
    rtl8139_device.mem_base = ret & (~0xf);
	
	
	rtl8139_device.tx_cur = 0;
	
	char iobase_mem_str[256];
	  
	hex_to_ascii(rtl8139_device.io_base, iobase_mem_str);
    kprint("HEX IO BASE: ");
    kprint(iobase_mem_str);
    kprint("\n");
	
	
	uint32_t  pci_command_reg = pci_read(pci_rtl8139_device, PCI_COMMAND);

	//Enable PCI Bus Mastering
	kprint("enable PCI Bust Mastering\n");
	if(!(pci_command_reg & (1 << 2))) {
		kprint("if enable pci bus mastering\n");
        pci_command_reg |= (1 << 2);
        pci_write(pci_rtl8139_device, PCI_COMMAND, pci_command_reg);
    }
	
	char pci_coman_reg_asci[256];
	  
	hex_to_ascii(pci_command_reg, pci_coman_reg_asci);
      
    kprint("HEX PCI COMMAND:: ");
    kprint(pci_coman_reg_asci);
    kprint("\n");
	
	kprint("CONFIG_1 register (0x52) to set the LWAKE + LWPTN\n");
    outportb(rtl8139_device.io_base + 0x52, 0x0);
	
	// Soft reset
	kprint("Soft reset;\n");
    outportb(rtl8139_device.io_base + 0x37, 0x10);
    while((inportb(rtl8139_device.io_base + 0x37) & 0x10) != 0) {
   
    }
	uint32_t phys_addr;
	rtl8139_device.rx_buffer = kmalloc(8192 + 16 + 1500, 1, &phys_addr);
	// Set RBStart to 0x10000(receiver buffer)
	outportl(rtl8139_device.io_base + 0x30,  rtl8139_device.rx_buffer );
	 
	// Sets the TOK and ROK bits high
	kprint("Set TOK and ROK high;\n");
    outports(rtl8139_device.io_base + 0x3C, 0x0005);
	
	kprint("Set RE and TE bits;\n");
	   // Sets the RE and TE bits high
    outportb(rtl8139_device.io_base + 0x37, 0x0C);
	 
	 	
	kprint("AB+AM+APM+AAP;\n");
	 // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
    outportl(rtl8139_device.io_base + 0x44, 0xF  );
	 
	
	kprint("configure rtl8139 interrupt handler\n");
	unsigned int irq_num = pci_read(pci_rtl8139_device, PCI_INTERRUPT_LINE);
    register_interrupt_handler(32+irq_num, rtl8139_handler);
	char irqNum_str[16];

	hex_to_ascii(irq_num, irqNum_str);
    kprint("HEX IRQ NUM: ");
    kprint(irqNum_str);
	kprint("\n");

	kprint("interrupt handler registered\n");
}


void read_mac_addr() 
{
	

	char buff[20];
    uint32_t mac_part1 = inportl(rtl8139_device.io_base + 0x00);
    uint16_t mac_part2 = inports(rtl8139_device.io_base + 0x04);
	
    rtl8139_device.mac_addr[0] = mac_part1 >> 0;
    rtl8139_device.mac_addr[1] = mac_part1 >> 8;
    rtl8139_device.mac_addr[2] = mac_part1 >> 16;
    rtl8139_device.mac_addr[3] = mac_part1 >> 24;
    rtl8139_device.mac_addr[4] = mac_part2 >> 0;
    rtl8139_device.mac_addr[5] = mac_part2 >> 8;
	
	kprint("read_mac_addr(): kprint: ");

	
	itoa(buff,rtl8139_device.mac_addr[0],16);
	kprint(buff);
	kprint(":");
		itoa(buff,rtl8139_device.mac_addr[1],16);
	kprint(buff);
	kprint(":");
		itoa(buff,rtl8139_device.mac_addr[2],16);
	kprint(buff);
	kprint(":");
		itoa(buff,rtl8139_device.mac_addr[3],16);
	kprint(buff);
	kprint(":");
		itoa(buff,rtl8139_device.mac_addr[4],16);
	kprint(buff);
	kprint(":");
		itoa(buff,rtl8139_device.mac_addr[5],16);
	kprint(buff);
	

}


void rtl8139_send_packet(void * data, uint32_t len) {
    // First, copy the data to a physically contiguous chunk of memory
   // void * transfer_data = kmalloc_a(len);
    //memcpy(transfer_data, data, len);
	kprint("\n");
	kprint("rtl8139_send_packet start\n");
	void* phys_addr = data;
    // Second, fill in physical address of data, and length
    outportl(rtl8139_device.io_base + TSAD_array[rtl8139_device.tx_cur], (uint32_t)data);
    outportl(rtl8139_device.io_base + TSD_array[rtl8139_device.tx_cur++], len);
    if(rtl8139_device.tx_cur > 3)
        rtl8139_device.tx_cur = 0;
	
	kprint("rtl8139_send_packet end\n");
}

void receive_packet() {
    uint16_t * t = (uint16_t*)(rtl8139_device.rx_buffer + current_packet_ptr);
    // Skip packet header, get packet length
    uint16_t packet_length = *(t + 1);
	char packet_length_str[16];
	
	
	hex_to_ascii(packet_length,packet_length_str);
	kprint("packet len:");
	kprint(packet_length_str);
    // Skip, packet header and packet length, now t points to the packet data
    t = t + 2;
    //qemu_printf("Printing packet at addr 0x%x\n", (uint32_t)t);
    //xxd(t, packet_length);

    // Now, ethernet layer starts to handle the packet(be sure to make a copy of the packet, insteading of using the buffer)
    // and probabbly this should be done in a separate thread...
   // void * packet = kmalloc(packet_length);
   // memcpy(packet, t, packet_length);
    //ethernet_handle_packet(packet, packet_length);

    current_packet_ptr = (current_packet_ptr + packet_length + 4 + 3) & RX_READ_POINTER_MASK; // Make sure current pointer 32 bits aligned

    if(current_packet_ptr > RX_BUF_SIZE) //Overflow happened
        current_packet_ptr -= RX_BUF_SIZE; //Set CAPR back at the beggining of the ring buffer

    outports(rtl8139_device.io_base + CAPR, current_packet_ptr - 0x10); // CAPR must point to an offset 16 bytes ahead of new receive packet header
}


void rtl8139_handler(registers_t r) {
    uint16_t status = inports(rtl8139_device.io_base + 0x3e);
	//uint16_t status;
	//kprint("rtl8139_handler() \n");
    if(status & TOK) {
        kprint("Packet sent\n");
    }
    if (status & ROK) {
        //kprint("Received packet\n");
        receive_packet();
    }


    outports(rtl8139_device.io_base + 0x3E, 0x0005);
}