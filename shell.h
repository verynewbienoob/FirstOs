#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdint.h>
extern int row_state;

void commands(char c);
uint8_t check_command();
void user_prompt(void);
void clear(void);

void no_command(void);
#endif