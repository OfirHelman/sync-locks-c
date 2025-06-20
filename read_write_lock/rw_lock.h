#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <stdatomic.h>

/*
 * Define the read-write lock type.
 * Write your struct details in this file..
 */

// ----------- Ticket Lock Definition -----------

typedef struct {
    atomic_int ticket;
    atomic_int current;
} ticket_lock;

void ticketlock_init(ticket_lock* lock);

void ticketlock_acquire(ticket_lock* lock);

void ticketlock_release(ticket_lock* lock);

// ----------- Read-Write Lock Definition -----------

typedef struct {
    // write your implementation here
    ticket_lock internal_lock;  
    int readers;                
    int writer_active;          
    int waiting_writers;
} rwlock;
 

/*
 * Initializes the read-write lock.
 */
void rwlock_init(rwlock* lock);

/*
 * Acquires the lock for reading.
 */
void rwlock_acquire_read(rwlock* lock);

/*
 * Releases the lock after reading.
 */
void rwlock_release_read(rwlock* lock);

/*
 * Acquires the lock for writing. This operation should ensure exclusive access.
 */
void rwlock_acquire_write(rwlock* lock);

/*
 * Releases the lock after writing.
 */
void rwlock_release_write(rwlock* lock);

#endif // RW_LOCK_H
