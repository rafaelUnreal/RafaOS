#include <stdint.h>

volatile uint16_t * const text_vid_buf = (uint16_t *)0xb8000;

int main() {
	
	

volatile uint16_t *start_ptr = text_vid_buf;
*start_ptr++ = 0x57 << 8 | 'R';
*start_ptr++ = 0x57 << 8 | 'a';
*start_ptr++ = 0x57 << 8 | 'f';
*start_ptr++ = 0x57 << 8 | 'a';
*start_ptr++ = 0x57 << 8 | 'O';
*start_ptr++ = 0x57 << 8 | 'S';



   return 0;
}