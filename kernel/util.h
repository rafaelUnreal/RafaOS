#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

void itoa(char *buf, unsigned long int n, int base);
void memory_copy(uint8_t *source, uint8_t *dest, int nbytes);
void memory_set(uint8_t *dest, uint8_t val, uint32_t len);
void hex_to_ascii(int n, char str[]);
uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr);
void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void append(char s[], char n);

#endif