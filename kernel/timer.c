#include "timer.h"
#include "../drivers/screen.h"
#include "util.h"
#include "idt.h"

unsigned int tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    //kprint("Tick: ");
    
    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
	
	kprint_at(tick_ascii,0,0);
    //kprint(tick_ascii);
    //kprint("\n");
}

void init_timer(unsigned int freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    unsigned int divisor = 1193180 / freq;
    unsigned short low  = (unsigned short)(divisor & 0xFF);
    unsigned short high = (unsigned short)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}