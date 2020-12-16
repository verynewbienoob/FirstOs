#ifndef STRING_H
#define STRING_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

char *itoa (int num, char *string, int base);
size_t strlen(const char* str);
void* memset (void *dest, uint32_t val, size_t len);
void memcpy (void *dest, void *src, size_t n);
uint8_t puts(const char *s);
uint32_t putchar(int c);
uint8_t inttostring(int number, char *buffer);
char *hexdump(int number, char *buffer);
int my_strcmp(const char *first, const char *second);
#endif