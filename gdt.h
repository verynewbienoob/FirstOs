#ifndef GDT_H
#define GDT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */

struct gdt_entry
{
    unsigned short limit_low; // size of the memory block that dscrptor references, 20 bits, split;
    unsigned short base_low; // adress where the block of memory that the descriptor references start, 32 bit value, split
    unsigned char base_middle;
    unsigned char access; //group of 8 flags
    unsigned char granularity; //0 - limit specified in bytes, 1 - blocks of 4 KB
    unsigned char base_high;
} __attribute__((packed));   // no optimization / packing

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern void gdt_flush(); //function in asm
void gdt_set_gate(uint8_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_init();
extern void gdt_flush();
#endif