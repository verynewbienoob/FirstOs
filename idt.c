#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

 /* We'll leave you to try and code this function: take the
 *  argument 'base' and split it up into a high and low 16-bits,
 *  storing them in idt[num].base_hi and base_lo. The rest of the
 *  fields that you must set in idt[num] are fairly self-
 *  explanatory when it comes to setup */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    /* Setup the first 16 bits of base adress */
    idt[num].base_low = (base & 0xFFFF);
    /* Setup the high 16 bits of base adress */
    idt[num].base_high = (base >> 16) & 0xFFFF;
    /* Setup A code segment selector in the GDT */
    idt[num].selector = sel;
    idt[num].zero = 0;
    /* Type and attributes of the descriptor */
    idt[num].flags = flags;
}
void idt_init()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' 
    Number of bytes (not entries) in the interrupt descriptor table, (to check if not) NOT minus one */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    /* Linear address of interrupt descriptor table */
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

   /* Points the processor's internal register to the new IDT */
    idt_load();
}