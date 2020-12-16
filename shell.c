#include <stdint.h>
#include "terminal.h"
#include "stdio.h"
#include "string.h" 

#include "shell.h"
#include "timer.h"

char command_buffer[128] = {0};
int row_state = 0;


char *available_commands[]=
{
    "c",
    "timer",
    NULL
};


void clear(void)
{
    terminal_initialize();
}


void (*fun_ptr[3])(void) = 
{
    clear,
    timer1
};

void no_command(void)
{
    terminal_setcolor(VGA_COLOR_LIGHT_RED);
    printf("Command not found :(\n");
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
}

uint8_t check_command()
{
    *(command_buffer + row_state) = '\0';   
    for (size_t i = 0; *(available_commands + i); i++)
    {
        if(my_strcmp(*(available_commands + i), command_buffer) == 0)
        {
            fun_ptr[i]();
            memset(command_buffer, 0, 128);
            return 1; // command found
        }   
    }
     // command not found
     //printf("Command not found :(\n");
     no_command();
     memset(command_buffer, 0, 128);
     row_state = 0;
     return -1;
}
void commands(char c)
{
    if( c == '\n')
    {
        check_command();
        user_prompt();
        return;
    }
    *(command_buffer + row_state) = c;
    row_state++;
}
void user_prompt(void)
{
    printf("user > ");
    row_state = 0;
    memset(command_buffer, 0, 128);
}