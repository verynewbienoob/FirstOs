#ifndef IDT_H
#define IDT_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>




/* Defines an IDT entry */
struct idt_entry
{
    uint16_t base_low;
    uint16_t selector; //kernel segment goes here
    uint8_t zero; // always zero
    uint8_t flags;
    uint16_t base_high;
}__attribute__((packed));

/* Defines an IDT pointer */
struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */


/* Asm function to load idt */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_init();
extern void idt_load();
#endif