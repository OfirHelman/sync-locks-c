#include "local_storage.h"
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>


/*
 * TODO: Define the global TLS array.
 */
ticket_lock tls_lock;
tls_data_t g_tls[MAX_THREADS];

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


/*
 * TODO: Implement init_storage to initialize g_tls.
 */
void init_storage(void) {
    // TODO: Set all thread_id fields to -1 and data pointers to NULL.
    ticketlock_init(&tls_lock);
    for (int i = 0; i < MAX_THREADS; ++i) {
        g_tls[i].thread_id = -1;
        g_tls[i].data = NULL;
    }
}

/*
 * TODO: Implement tls_thread_alloc to allocate a TLS entry for the calling thread.
 */
void tls_thread_alloc(void) {
    // TODO: Use your synchronization mechanism to safely allocate an entry.
     int64_t tid = (int64_t)pthread_self();  

    ticketlock_acquire(&tls_lock);  

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (g_tls[i].thread_id == tid) {
            ticketlock_release(&tls_lock); 
            return;
        }
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (g_tls[i].thread_id == -1) {
            g_tls[i].thread_id = tid;
            g_tls[i].data = NULL;
            ticketlock_release(&tls_lock);
            return;
        }
    }

    fprintf(stderr, "thread [%ld] failed to initialize, not enough space\n", tid);
    ticketlock_release(&tls_lock);
    exit(1);  
}

/*
 * TODO: Implement get_tls_data to retrieve the TLS data for the calling thread.
 */
void* get_tls_data(void) {
    // TODO: Search for the calling thread's entry and return its data.
    int64_t tid = (int64_t)pthread_self();  

    ticketlock_acquire(&tls_lock);  

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (g_tls[i].thread_id == tid) {
            void* data = g_tls[i].data;
            ticketlock_release(&tls_lock);
            return data;
        }
    }

    
    fprintf(stderr, "thread [%ld] hasn't been initialized in the TLS\n", tid);
    ticketlock_release(&tls_lock);
    exit(2);
}



/*
 * TODO: Implement set_tls_data to update the TLS data for the calling thread.
 */
void set_tls_data(void* data) {
    // TODO: Search for the calling thread's entry and set its data.
    int64_t tid = (int64_t)pthread_self();  

    ticketlock_acquire(&tls_lock);  

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (g_tls[i].thread_id == tid) {
            g_tls[i].data = data;
            ticketlock_release(&tls_lock);
            return;
        }
    }

    fprintf(stderr, "thread [%ld] hasn't been initialized in the TLS\n", tid);
    ticketlock_release(&tls_lock);
    exit(2);  
}

/*
 * TODO: Implement tls_thread_free to free the TLS entry for the calling thread.
 */
void tls_thread_free(void) {
    // TODO: Reset the thread_id and data in the corresponding TLS entry.
      int64_t tid = (int64_t)pthread_self();  

    ticketlock_acquire(&tls_lock);  

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (g_tls[i].thread_id == tid) {
            g_tls[i].thread_id = -1;  
            g_tls[i].data = NULL;     
            ticketlock_release(&tls_lock);
            return;
        }
    }

    fprintf(stderr, "thread [%ld] hasn't been initialized in the TLS\n", tid);
    ticketlock_release(&tls_lock);
    exit(2);  
}
