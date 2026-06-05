#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "hardware_processing.h"

// -----------------------------------------------------------------------------
// PACKET TYPES
// -----------------------------------------------------------------------------
typedef enum
{
    PACKET_UNKNOWN = 0,
    PACKET_USB,
    PACKET_KEYBOARD

} packet_type_t;

typedef enum
{
    EVENT_NONE = 0,
    EVENT_SIZE_PACKET_RECIEVED,
    EVENT_PACKET_COMPLETE,
    EVENT_USB_DETECTED,
    EVENT_KEYBOARD_DETECTED,
    EVENT_KEYBOARD_CHAR_PRESSED,
    EVENT_PROCESSING_COMPLETE
} event_type_t;

typedef enum
{
    STATE_IDLE = 0,
    STATE_RECEIVING_HEADER,
    STATE_CLASSIFYING,
    STATE_RECEIVING_PAYLOAD,
    STATE_PROCESSING_USB,
    STATE_PROCESSING_KEYBOARD

} packet_state_t;


// -----------------------------------------------------------------------------
// EXTERN VARIABLES 
// -----------------------------------------------------------------------------

extern volatile packet_type_t current_packet;
extern volatile packet_state_t define_state;

// -----------------------------------------------------------------------------
// QUEUE INITIALIZATION
// -----------------------------------------------------------------------------

PUBLIC void queue_init(void);
PUBLIC bool enqueue(event_type_t event);
PUBLIC bool dequeue(event_type_t *event);
PUBLIC bool enqueue_keyboard(uint8_t letter);
PUBLIC bool is_keyboard_empty(void);
// -----------------------------------------------------------------------------
// BUFFER ACCESS
// -----------------------------------------------------------------------------

PUBLIC uint8_t *give_array_address(void);

PUBLIC uint8_t *give_array_address_for_file_writing(void);

PUBLIC int get_queue_size(void);

// -----------------------------------------------------------------------------
// PACKET CLASSIFICATION
// -----------------------------------------------------------------------------

PUBLIC packet_type_t classify_packet(void);

// -----------------------------------------------------------------------------
// OPTIONAL HELPERS
// -----------------------------------------------------------------------------

PUBLIC void set_array_index(int difference);