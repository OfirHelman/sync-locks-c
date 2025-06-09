#include "tas_semaphore.h"
#include <sched.h>

int test_and_set(int *lock)
{
    int old = *lock;
    *lock = 1;
    return old;
}

/*
 * TODO: Implement semaphore_init using a TAS spinlock.
 */
void semaphore_init(semaphore *sem, int initial_value)
{
    // TODO: Define the structure and initialize the semaphore.

    sem->value = initial_value; 
    sem->lock = 0;              
}

/*
 * TODO: Implement semaphore_wait using the TAS spinlock mechanism.
 */
void semaphore_wait(semaphore* sem) {
    while (1) {
        while (test_and_set(&(sem->lock)))
            sched_yield(); 

        if (sem->value > 0) {
            sem->value--;    
            sem->lock = 0;   
            return;          
        }

        sem->lock = 0; 
    }
}


/*
 * TODO: Implement semaphore_signal using the TAS spinlock mechanism.
 */
void semaphore_signal(semaphore *sem)
{
// TODO: Acquire the spinlock, increment the semaphore value, then release the spinlock.
 // Only if you want to use sched_yield()

    while (test_and_set(&(sem->lock)))
        sched_yield(); 
    sem->value++;  
    sem->lock = 0; 
}
