#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "gdt.h"

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gp;

// configuring code descriptor [1] since [0] is NULL descriptor

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(uint8_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
} 

void gdt_init()
{
    //Setup GDT pointer and limit
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = &gdt;

    //null descriptor
    gdt_set_gate(0,0,0,0,0);
    //code descriptor
    gdt_set_gate(1,0,0xFFFFFFFF,0x9a,0xcf);
    //data descriptor
    gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xcf);
    gdt_flush();
}
