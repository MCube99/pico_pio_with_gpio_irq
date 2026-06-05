
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "queue.h"

#include "hardware/dma.h"

#include "hardware_processing.h"
#include "hardware/uart.h"
#include "hardware/sync.h"

// -----------------------------------------------------------------------------
// PRIVATE FUNCTION STORE
// -----------------------------------------------------------------------------
PRIVATE uint8_t dequeue_keyboard(void)
// -----------------------------------------------------------------------------
// QUEUE STORAGE
// -----------------------------------------------------------------------------

struct queue_type {
    event_type_t queue[16]; //arbitrary number picked. Just a cheeky queue handler for interrupts
    volatile uint8_t front;
    volatile uint8_t rear;
};

struct keyboard_queue_type {
    char queue[16]; //arbitrary number picked. Just a cheeky queue handler for interrupts
    volatile uint8_t front;
    volatile uint8_t rear;
};

static struct queue_type myQueue;
static struct keyboard_queue_type keyboardQueue;
static BYTE buffer[BUF_LEN];


// -----------------------------------------------------------------------------
// BUFFER ACCESSORS
// -----------------------------------------------------------------------------

PUBLIC uint8_t *give_array_address(void) {
    return &buffer[0];
}

PUBLIC uint8_t *give_array_address_for_file_writing(void) {
    return &buffer[1];
}

PUBLIC int get_buffer_size(void) {
    return buffer[0];
}


// -----------------------------------------------------------------------------
// INTERRUPT INITIALISATION(QUEUES)
// -----------------------------------------------------------------------------
PUBLIC void queue_init(void) {
    memset(myQueue.queue, 0, 16);
    memset(keyboardQueue.queue, 0, 16);
    myQueue.front = 0;
    myQueue.rear = 0;
    keyboardQueue.front = 0;
    keyboardQueue.rear = 0;
}

PUBLIC bool enqueue(event_type_t event) {
    if (((myQueue.rear + 1) % 16) == myQueue.front) {
        return false;    // Queue Full
    }

    myQueue.rear = (myQueue.rear + 1) % 16;
    myQueue.queue[myQueue.rear] = event;
    return true;
}

PUBLIC bool dequeue(event_type_t *event) {
    if (myQueue.front == myQueue.rear) {
        return false;    // Queue Empty
    }
    myQueue.front = (myQueue.front + 1) % 16;
    *event = myQueue.queue[myQueue.front];
    return true;
}

// -----------------------------------------------------------------------------
// KEYBOARD PACKET READ INITIALISATION(QUEUES). THIS WILL STORE DATA UNTIL ITS TIME to crap it out
// -----------------------------------------------------------------------------

PUBLIC bool enqueue_keyboard(uint8_t letter) {
    if (((keyboardQueue.rear + 1) % 16) == keyboardQueue.front) {
        return false;    // Queue Full
    }

    keyboardQueue.rear = (keyboardQueue.rear + 1) % 16;
    keyboardQueue.queue[keyboardQueue.rear] = letter;
    return true;
}

PUBLIC bool is_keyboard_empty(void)
{
    return (keyboardQueue.front == keyboardQueue.rear);
}

PRIVATE uint8_t dequeue_keyboard(void)
{
    keyboardQueue.front =
        (keyboardQueue.front + 1) % 16;

    return keyboardQueue.queue[keyboardQueue.front];
}

PUBLIC void keyboard_processing(void)
{
  uint8_t ch = dequeue_keyboard();
  if(ch == '/r')
  {
    pio_sm_put()
  } 
}
// -----------------------------------------------------------------------------
// PACKET CLASSIFICATION
// -----------------------------------------------------------------------------

PUBLIC void classify_packet(void) {
    uint32_t size = get_queue_size();

    if ((size == GARY_CODE) || (size == 0))
    {
        current_packet = PACKET_KEYBOARD;
        pio_sm_put( return_spi_pio(), return_spi_sm(), 0);
        enqueue(EVENT_KEYBOARD_DETECTED);
    }
    else
    {
        current_packet = PACKET_USB;

        enqueue(EVENT_USB_DETECTED);
    }
}

PUBLIC void check_usb_transfer() {

    uintptr_t base = (uintptr_t)&buffer[0];

    uintptr_t write = dma_hw->ch[return_channel()].write_addr;

    uint32_t difference = write - base;

    if(difference ==  return_size())
    {
        usb_transfer_done = true;
        current_packet = PACKET_NONE;
    }
}