#include <stdbool.h>
#include <stdint.h>
#include "string.h"
#include "timer.h"
#include "isrs.h"
#include "kboard.h"
#include "pc.h"
#include "shell.h"

/* This will keep track of how many ticks that the system
*  has been running for */
uint32_t timer_ticks = 0;


void timer_phase(int hz)
{
    uint32_t divisor = 1193180 / 500; ;    ; // (66287 gives us almost a default 18.2222hz) calculates divisor ( how many ticks per second )
    outportb(0x43,0x36); /* 0x43 is command register, 0x36  is our byte defined:
                         CNTR = 0, RW mode = 3, MODE = 3, BCD = 0 (16 bit counter) */
    outportb(0x40, divisor & 0xFF ); /*setting low byte of divisor, 0x40 - data register port for channel 0 */
    outportb(0x40, (divisor >> 8) ); /* setting high byte */                       
}

void timer_handler(struct regs *r)
{

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    // if (timer_ticks % 500 ==  0)
    // {
       
    //    puts(" One second has passed\n");
    // }
    // printf("%d ", timer_ticks);
    timer_ticks++;

    kb_queue_handler();

}


/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    timer_phase(2);
    irq_install_handler(0, timer_handler);
}

void sleepp(uint32_t time)
{
    volatile unsigned long desired_ticks;
    desired_ticks = timer_ticks + time;
    while(timer_ticks < desired_ticks)
    {
        //printf("%d :: %d\n",timer_ticks, desired_ticks);
        asm("pause");
    }
    return;
}

void timer1()
{
    volatile uint32_t clock_ticks = 0;
    for (;clock_ticks < 10;)
    {
        sleepp(100);
        clock_ticks++;
        terminal_display_at(clock_ticks,0, 74);
    }
}
// void timer2()
// {
//     volatile uint32_t clock_ticks = 0;
//     for (;clock_ticks < 10;)
//     {
//         // sleepp(100);
//         // clock_ticks++;
//         // terminal_display_at(clock_ticks,0, 74);
//         printf("cc\n");
//     }
// }

