#include "tl_semaphore.h"
#include <sched.h>

/*
 * TODO: Implement semaphore_init for the Ticket Lock semaphore.
 */

    void semaphore_init(tl_semaphore* sem, int initial_value) {
    sem->value = initial_value;
    ticketlock_init(&(sem->lock));
    }

    // TODO: Define the structure and initialize the semaphore.

/*
 * TODO: Implement semaphore_wait using the Ticket Lock mechanism.
 */
    // TODO: Obtain a ticket and wait until it is your turn; then decrement the semaphore value.
    void semaphore_wait(tl_semaphore* sem) {
        while (1) {
            ticketlock_acquire(&(sem->lock));
    
            if (sem->value > 0) {
                sem->value--;
                ticketlock_release(&(sem->lock));
                break;
            }
    
            ticketlock_release(&(sem->lock));
            sched_yield(); 
        }
    }


/*
 * TODO: Implement semaphore_signal using the Ticket Lock mechanism.
 */

    // TODO: Increment the semaphore value and allow the next ticket holder to proceed.
    void semaphore_signal(tl_semaphore* sem) {
        ticketlock_acquire(&(sem->lock));
        sem->value++;
        ticketlock_release(&(sem->lock));
    }
    

