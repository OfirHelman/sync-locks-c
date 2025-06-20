#ifndef TL_SEMAPHORE_H
#define TL_SEMAPHORE_H
#include "ticket_lock.h"
#include <stdatomic.h>

/*
 * Define the semaphore type for the Ticket Lock implementation.
 * Write your struct details in this file..
 */
typedef struct {
    int value;
    ticket_lock lock;
     // write your implementation here
} tl_semaphore;

/*
 * Initializes the semaphore pointed to by 'sem' with the specified initial value.
 */
void semaphore_init(tl_semaphore* sem, int initial_value);

/*
 * Decrements the semaphore (wait operation).
 */
void semaphore_wait(tl_semaphore* sem);

/*
 * Increments the semaphore (signal operation).
 */
void semaphore_signal(tl_semaphore* sem);

#endif // TL_SEMAPHORE_H
