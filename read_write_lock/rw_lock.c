#include "rw_lock.h"
#include <sched.h>

// ----------- Ticket Lock Implementation -----------

void ticketlock_init(ticket_lock* lock) {
    atomic_init(&lock->ticket, 0);
    atomic_init(&lock->current, 0);
}

void ticketlock_acquire(ticket_lock* lock) {
    int my_ticket = atomic_fetch_add(&lock->ticket, 1);
    while (atomic_load(&lock->current) != my_ticket) {
        sched_yield();
    }
}

void ticketlock_release(ticket_lock* lock) {
    atomic_fetch_add(&lock->current, 1);
}

// ----------- Read-Write Lock Implementation -----------

/*
 * TODO: Implement rwlock_init.
 */

void rwlock_init(rwlock* lock) {
    // TODO: Initialize the lock structure.
    ticketlock_init(&lock->internal_lock);
    lock->readers = 0;
    lock->writer_active = 0;
    lock->waiting_writers = 0;

}

/*
 * TODO: Implement rwlock_acquire_read.
 */
void rwlock_acquire_read(rwlock* lock) {
    while (1) {
        ticketlock_acquire(&lock->internal_lock);
        if (lock->writer_active == 0) {
            lock->readers++;
            ticketlock_release(&lock->internal_lock);
            return;
        }
        ticketlock_release(&lock->internal_lock);
        sched_yield();
    }
}



/*
 * TODO: Implement rwlock_release_read.
 */
void rwlock_release_read(rwlock* lock) {
    // TODO: Decrement the reader count.
    ticketlock_acquire(&lock->internal_lock);
    lock->readers--;
    ticketlock_release(&lock->internal_lock);
}

/*
 * TODO: Implement rwlock_acquire_write.
 */
void rwlock_acquire_write(rwlock* lock) {
    // TODO: Ensure exclusive access for writing.
    ticketlock_acquire(&lock->internal_lock);
    lock->waiting_writers++;
    ticketlock_release(&lock->internal_lock);

    while (1) {
        ticketlock_acquire(&lock->internal_lock);
        if (lock->readers == 0 && lock->writer_active == 0) {
            lock->writer_active = 1;
            lock->waiting_writers--;
            ticketlock_release(&lock->internal_lock);
            return;
        }
        ticketlock_release(&lock->internal_lock);
        sched_yield();
    }
}

/*
 * TODO: Implement rwlock_release_write.
 */
void rwlock_release_write(rwlock* lock) {
    // TODO: Release the write lock.
    ticketlock_acquire(&lock->internal_lock);
    lock->writer_active = 0;
    ticketlock_release(&lock->internal_lock);
}
