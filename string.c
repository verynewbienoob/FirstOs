#include "string.h"
#include "terminal.h"
#include <stdbool.h>
#include <stdint.h>


size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void *memset (void *dest, uint32_t val, size_t len)
{
	uint8_t *ptr = dest;
	for (size_t i = 0; i < len; i++)
		*(ptr + i) = val;
	return dest;
}
void memcpy (void *dest, void *src, size_t n)
{
	char *_src = (char *)src;
	char *_dest = (char *)dest;

	for (size_t i = 0; i < n ; i++)
		*(_dest + i) = *(_src + i);
	return;	
}

uint8_t puts(const char* s)
{
	if( s == NULL)
		return -1;
	size_t length = strlen(s);
	for (size_t i = 0; i < length; i++)
		terminal_putchar(*(s + i));		
	terminal_putchar('\n');
	return 1;	
}

uint32_t putchar (int c)
{
	if(c >= 0)
	{
		// if(c == '\b')
		// {
		// 	back_space();
		// 	return c;
		// }
		terminal_putchar((unsigned char)c);
		return c;
	}
	else
		return -1;
}
char *itoa (int num, char *string, int base)
{
	char *ptr = string;
	if(num == 0)
	{
		*(string) = '0';
		*(string + 1) = '\0';
		return string;
	}
	int i = 0 ;
	if( num < 0 && base == 10) // is negative but only base 10
		num *= -1;
	
	for (i; num != 0; i++)
	{
		int rem = num % base;
		*(string + i) = (rem <= 9) ? (rem + '0') : (rem - 10 + 'A');
		num /= base;
	}
	*(string + i ) = '\0';
	//reverse the string;
	for (size_t j = 0, f = i - 1; j < i/2; j++, f--)
	{
		char tmp;
		tmp = string[j];
		string[j] = string[f];
		string[f] = tmp;
	}
   return string;
}
int my_strcmp(const char *first, const char *second)
{
	while(*first == *second)
	{
		if( *first == '\0' || *second == '\0')
			break;
		first++;
		second++;
	}
	if(*first == '\0' && *second == '\0')
		return 0;
	return -1;
}


char *hexdump(int num, char *buffer) 
{
   int idx = 0;
   int size = 0;
   while (num != 0) {
      int digit = num % 16;
      char c = (digit <= 9) ? '0'+digit : 'A'+digit-10;
      buffer[idx++] = c;
      num /= 16;
	  size++;
   }
   buffer[idx] = 'x';
   buffer[++idx] = '0';
   size += 2;
	// make it backward
	for (size_t i = 0, j = size - 1; i < size/2; i++, j--)
	{
		char tmp;
		tmp = buffer[i];
		buffer[i] = buffer [j];
		buffer[j] = tmp;
	}
   return buffer;
}