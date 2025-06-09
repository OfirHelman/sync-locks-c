#ifndef LOCAL_STORAGE_H
#define LOCAL_STORAGE_H

#include <stdint.h>
#include <pthread.h>
#include <stdatomic.h>


#define MAX_THREADS 100

/*
 * Ticket lock for synchronization
 */
typedef struct {
    atomic_int ticket;
    atomic_int current;
} ticket_lock;

void ticketlock_init(ticket_lock* lock);
void ticketlock_acquire(ticket_lock* lock);
void ticketlock_release(ticket_lock* lock);


/*
 * Structure to hold thread-specific arbitrary data.
 */
typedef struct {
    int64_t thread_id;  
    void* data;         
} tls_data_t;

/*
 * Global TLS array.
 * Students should define this array in local_storage.c.
 */
extern tls_data_t g_tls[MAX_THREADS];

/*
 * Initializes g_tls as an array of MAX_THREADS entries.
 */
void init_storage(void);

/*
 * Initializes the TLS entry for the calling thread.
 */
void tls_thread_alloc(void);

/*
 * Returns a pointer to the arbitrary data stored for the calling thread.
 */
void* get_tls_data(void);

/*
 * Sets the given void* data to the TLS for the calling thread.
 */
void set_tls_data(void* data);

/*
 * Frees the TLS entry for the calling thread.
 */
void tls_thread_free(void);

#endif // LOCAL_STORAGE_H
