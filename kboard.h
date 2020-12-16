#ifndef KBOARD_H
#define KBOARD_H
#include "isrs.h"

#define SIZE 18
enum kboard_ports
{
    KBOARD_ENCODERWRITE = 0x60,
    KBOARD_ENCODERREAD = 0x60,
    KBOARD_CTRLREAD = 0x64,
    KBOARD_CTRLWRITE = 0x64
};
/*  
    Bit 0: Output Buffer Status
    0: Output buffer empty, dont read yet
    1: Output buffer full, please read me :)

    Bit 1: Input Buffer Status
    0: Input buffer empty, can be written
    1: Input buffer full, dont write yet
*/
enum kboard_ctrl_status_mask
{
    KBOARD_CTRL_OUTBUFF_MASK = 1,
    KBOARD_CTRL_INPUTBUFF_MASK = 2
};
struct kboard_queue_t
{
    uint8_t tail; /* tail */
    uint8_t head;   /* head */ 
    uint8_t full : 1;
    uint8_t empty : 1;
    //size is defined
    struct kboard_event_t *events;
};
struct kboard_event_t
{
    uint8_t scancode;
    uint8_t used: 1; // indicate if we try to pop on empty buffer 0 - not used  1 - used
    uint8_t pressed : 1;  // 0 - not pressed, 1 - pressed
    uint8_t released : 1; // 0 - not released, 1 - released
};




void kboard_handler(struct regs *r);
void kboard_install();
/* Function used to read ask onboard controller bout 
output/input buffer status(other flags are unimportant for now) */
uint8_t kboard_status_check();

/* Queue implementation based on circular buffer */
void kboard_queue_init();
void queue_push(struct kboard_event_t key_pressed);
struct kboard_event_t queue_pop ();
uint8_t kboard_empty();
/* function that handles the queue, I will attach that to timer */
void kb_queue_handler();

#endif