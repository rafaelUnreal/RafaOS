#ifndef RTL8139_H
#define RTL8139_H

#include "pci.h"
#include "idt.h"
#include <stdint.h>

#define CAPR 0x38
#define RX_READ_POINTER_MASK (~3)
#define ROK                 (1<<0)
#define RER                 (1<<1)
#define TOK     (1<<2)
#define TER     (1<<3)
#define TX_TOK  (1<<15)

#define RX_BUF_SIZE 8192


#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139

 
#define REG_CTRL        0x0000
#define REG_STATUS      0x0008
#define REG_EEPROM      0x0014
#define REG_CTRL_EXT    0x0018
#define REG_IMASK       0x00D0
#define REG_RCTRL       0x0100
#define REG_RXDESCLO    0x2800
#define REG_RXDESCHI    0x2804
#define REG_RXDESCLEN   0x2808
#define REG_RXDESCHEAD  0x2810
#define REG_RXDESCTAIL  0x2818


typedef struct rtl8139_dev {
    uint8_t bar_type;
    uint16_t io_base;
    uint32_t mem_base;
    int eeprom_exist;
    uint8_t mac_addr[6];
    char *rx_buffer;
    int tx_cur;
}rtl8139_dev_t;

uint8_t TSAD_array[4]; 
uint8_t TSD_array[4];

pci_dev_t pci_rtl8139_device;
rtl8139_dev_t rtl8139_device;
 
void read_mac_addr(); 
void rtl8139_send_packet(void * data, uint32_t len);
void rtl8139_handler(registers_t r);
void rtl8139_init();

 
 
#endif