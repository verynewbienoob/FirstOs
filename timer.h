#ifndef TIME_H
#define TIME_H
#include "isrs.h"
extern uint32_t timer_ticks;
void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();
void sleepp(uint32_t time);
void timer1();
void timer2();
#endif