#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "string.h"
#include "kernel.h"
#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "shell.h"
#include "kboard.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

/* Check if any text is written to terminal. row_state is variable in shell.c */
void back_space()
{
	if(row_state == 0) 
	{
		return;
	}	
	--terminal_column;
	terminal_buffer[( terminal_row * VGA_WIDTH ) + terminal_column] = vga_entry(' ', terminal_color);
	fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column );
	row_state--;
}


#define FB_HIGH_BYTE_COMMAND 14 //command code for asm,14 tells framebuffer to expect the highest 8 bits of the position
#define FB_LOW_BYTE_COMMAND  15//command code for asm, 15 tells the framebuffer to expect the lowest 8 bits of position
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5
void fb_move_cursor(unsigned short pos)
{
    movcur(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    movcur(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    movcur(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    movcur(FB_DATA_PORT,    pos & 0x00FF);
}

void terminal_display_at(int thing, uint8_t x, uint8_t y)
{
	int index = x * VGA_WIDTH + y;
	char time[5] = {0};
	int old_row = terminal_row;
	int old_col = terminal_column;
	terminal_row = x;
	terminal_column = y;
	printf("%d",thing);
	terminal_row = old_row;
	terminal_column = old_col;
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) 
{
	    
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
void terminal_scroll(void)
{
	for (size_t i = 0; i < VGA_HEIGHT - 1 ; i++)
	{
		for (size_t j = 0; j < VGA_WIDTH; j++)
		{
			//Copy glyph by glyph
			terminal_buffer[i * VGA_WIDTH + j] = terminal_buffer[i * VGA_WIDTH + j + VGA_WIDTH];
		}
		
	}
	for (size_t i = 0; i < VGA_WIDTH ; i++)
	{
		//Do the same as in initialization but only for last row.
		terminal_buffer[VGA_WIDTH * (VGA_HEIGHT - 1) + i] = vga_entry(' ', terminal_color);
	}
		
	terminal_column = 0;
	terminal_row -= 1;
	
}

void terminal_putchar(char c) 
{
	if( c == '\n')
	{
		if(++terminal_row  == VGA_HEIGHT)
		{
			terminal_scroll();
		}	
		terminal_column = 0;
		fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
		
	}
	else
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH)
		{
			if(++terminal_row == VGA_HEIGHT)
				terminal_scroll();
			terminal_column = 0;
			fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
		}
		fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);

	}
	
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}