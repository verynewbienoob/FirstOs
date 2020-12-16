#include <stdint.h>
#include <stdbool.h>
#include "kboard.h"
#include "string.h"
#include "isrs.h"
#include "pc.h"
#include "shell.h"
#include "terminal.h"
#include "timer.h"
unsigned char kbdus_lowercase[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};
unsigned char kbdus_uppercase[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '\\', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};
uint8_t kboard_status_check()
{
    return inportb(KBOARD_CTRLREAD);
}

/* QUEUE IMPLEMENTATION */  
struct 
{
    uint8_t shift_left : 1;
    uint8_t shift_right : 1;
    uint8_t ctrl_left : 1;
    uint8_t caps : 1;
    uint8_t alt : 1;
} special_keys;

struct kboard_queue_t kboard_queue;
struct kboard_event_t keys_queue[SIZE] = {0};

void kboard_queue_init()
{
    kboard_queue.full = 0;
    kboard_queue.empty = 0;
    kboard_queue.tail = 0;
    kboard_queue.head = 0;
    kboard_queue.events = keys_queue;
}

void queue_push(struct kboard_event_t key_pressed)
{
    /*check if queue is full */
    if (kboard_queue.full == 1)
        kboard_queue.tail =(kboard_queue.tail + 1) % SIZE;

    if (kboard_queue.empty == 1)
        kboard_queue.empty = 0;

    keys_queue[kboard_queue.head] = key_pressed;    
    kboard_queue.head = (kboard_queue.head + 1) % SIZE;

    kboard_queue.full = (kboard_queue.head == kboard_queue.tail );
}

struct kboard_event_t queue_pop ()
{
    if ( kboard_queue.empty == 1)
        return (struct kboard_event_t) {0};    
    if ( kboard_queue.full == 1 )
        kboard_queue.full = 0;

    struct kboard_event_t pop_event = kboard_queue.events[kboard_queue.tail];
    kboard_queue.tail = (kboard_queue.tail + 1 ) % SIZE; 

    if(kboard_queue.full != 1 && (kboard_queue.tail == kboard_queue.head))
        kboard_queue.empty = 1;

    return pop_event;
}

void kb_queue_handler()
{
  struct kboard_event_t key;
  while(1)
  {
    key = queue_pop();
    if (!key.used) // buffer is empty
      break;
    if( key.pressed )
    {
      switch (key.scancode)
      {
        case 0x2a:
          special_keys.shift_left = 1;
          return;
        case 0x1d:
          special_keys.ctrl_left = 1;
          return;  
        case 0x3a:
          special_keys.caps = 1;
          return;
        default:
          break;
        // rest special keys TODO 
      }
      if( key.scancode == 0x0e)
      {
          back_space();
          break;
      }
      else if(special_keys.shift_left || special_keys.caps )
      {
          putchar(kbdus_uppercase[key.scancode]);
          commands(kbdus_uppercase[key.scancode]); // shell buffer
          break;
      }
      else
      {
          putchar(kbdus_lowercase[key.scancode]);
          commands(kbdus_lowercase[key.scancode]);  // shell buffer 
          break;
      }
    }
    else if (key.released)
    {
       switch (key.scancode)
        {
          case 0xaa:
            special_keys.shift_left = 0;
            break;
          case 0x9d:
            special_keys.ctrl_left = 0;
            break;  
          case 0xba:
            special_keys.caps = 0;
            break;
          default:
            break;
          // rest special keys TODO 
        }  
    }
    
  }
}



void kboard_handler(struct regs *r)
{ 
    // wait for keyboard to be ready
    while (1)
      if ( kboard_status_check() & (KBOARD_CTRL_OUTBUFF_MASK == 1) )
        break;
        
    uint8_t scancode = inportb(KBOARD_ENCODERREAD);
    struct kboard_event_t new_event;
    new_event.scancode = scancode;
    new_event.used = 1;
    new_event.pressed = (scancode < 0x80);
    new_event.released = (scancode >= 0x80);
    queue_push(new_event);

}

uint8_t kboard_empty()
{
    if(kboard_queue.empty == 1)
      return 1;
    return 0;
}

void kboard_install()
{
 
    irq_install_handler(1, kboard_handler);

}