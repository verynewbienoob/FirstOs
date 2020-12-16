#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "string.h"
#include "kernel.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "isrs.h"
#include "timer.h"
#include "kboard.h"
#include "stdio.h"
#include "shell.h"

 
void kernel_main(void) 
{
	/* Initialize GDT */
	
	gdt_init();
	idt_init();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	
	
	
	
	
	terminal_initialize();
	user_prompt();
	timer_install();

	kboard_queue_init();
	kboard_install();





	

	

	




}
