#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "ring_buf.h"
#include "hardware_processing.h"

// -----------------------------------------------------------------------------
// PACKET TYPES
// -----------------------------------------------------------------------------

typedef enum
{
    EVENT_NONE=0,
    EVENT_CSN_ASSERTED,
    EVENT_SIZE_PACKET_RECIEVED,
    EVENT_USB_DETECTED,
    EVENT_USB_PROCESSING,
    EVENT_FILE_PROCESSING,
    EVENT_FILE_PROCESSED,
    EVENT_KEYBOARD_DETECTED,
} event_type_t;

<<<<<<< HEAD
typedef enum
{
    STATE_WAIT_FOR_SIZE,
    STATE_WAIT_FOR_USB_DATA,
    STATE_WAIT_FOR_KEYBOARD_DATA,
    STATE_LEGIT_CHARACTERS_INPUTTED,
    STATE_ENTER_INPUTTED
} protocol_state_t;

extern volatile protocol_state_t protocol_state;
extern volatile bool keyboard_check;
=======
extern volatile event_type_t event;
extern volatile bool main_check;
>>>>>>> 0e60d90fa300e0030290cad6ebbbe990d68fdfd0


PUBLIC void classify_packet(void);
PUBLIC void keyboard_processing(void);

// -----------------------------------------------------------------------------
// QUEUE INITIALIZATION
// -----------------------------------------------------------------------------

PUBLIC void queue_init(void);
//PUBLIC bool is_queue_empty(void); 
PUBLIC bool enqueue_interrupts(event_type_t event);
PUBLIC bool enqueue_keyboard(uint8_t letter);

PUBLIC bool dequeue_interrupts(event_type_t *event);
PUBLIC bool dequeue_keyboard(uint8_t *letter);
//PUBLIC* RingBuf return_interrupt_queue();
