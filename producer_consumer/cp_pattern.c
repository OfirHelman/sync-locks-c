#include "cp_pattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"


// ===== Globals =====
queue_t shared_queue;
ticket_lock queue_lock;
condition_variable not_empty;

pthread_t* producer_threads = NULL;
pthread_t* consumer_threads = NULL;

atomic_int produced_count;
atomic_int consumed_count;
atomic_int producers_finished;

void print_msg(const char* msg) {
    ticketlock_acquire(&queue_lock);
    printf("%s", msg);
    fflush(stdout);
    ticketlock_release(&queue_lock);
}

void wait_until_producers_produced_all_numbers() {
    while (atomic_load(&produced_count) < MAX_NUMBER) {
        sched_yield();
    }
}

void wait_consumers_queue_empty() {
    while (!queue_is_empty(&shared_queue)) {
        sched_yield();
    }
}

void* consumer_thread_func(void* arg) {
    while (1) {
        ticketlock_acquire(&queue_lock);

        while (queue_is_empty(&shared_queue)) {
            if (atomic_load(&producers_finished) && queue_is_empty(&shared_queue)) {
                ticketlock_release(&queue_lock);
                return NULL;
            }
            condition_variable_wait(&not_empty, &queue_lock);
        }

        int value;
        if (queue_dequeue(&shared_queue, &value)) {
            atomic_fetch_add(&consumed_count, 1);
        }

        ticketlock_release(&queue_lock);

        char msg[150];
        sprintf(msg, "Consumer %ld checked %d. Is it divisible by 6? %s\n",
        (long)pthread_self(), value, (value % 6 == 0) ? "True" : "False");
        print_msg(msg);
    }

    return NULL;
}


void* producer_thread_func(void* arg) {
    while (1) {
        int current = atomic_fetch_add(&produced_count, 1);
        if (current >= MAX_NUMBER) {
            break;
        }

        char msg[100];
        sprintf(msg, "Producer %ld generated number: %d\n", (long)pthread_self(), current);

        print_msg(msg);

        ticketlock_acquire(&queue_lock);
        queue_enqueue(&shared_queue, current);
        condition_variable_broadcast(&not_empty);
        ticketlock_release(&queue_lock);
    }

    return NULL;
}



void start_consumers_producers(int consumers, int producers, int seed) {
    printf("Number of Consumers: %d\n", consumers);
    printf("Number of Producers: %d\n", producers);
    printf("Seed: %d\n", seed);

    srand(seed);

    queue_init(&shared_queue);
    ticketlock_init(&queue_lock);
    condition_variable_init(&not_empty);

    atomic_init(&produced_count, 0);
    atomic_init(&consumed_count, 0);
    atomic_init(&producers_finished, 0);

    producer_threads = malloc(sizeof(pthread_t) * producers);   
    consumer_threads = malloc(sizeof(pthread_t) * consumers);

    for (int i = 0; i < producers; ++i) {
        pthread_create(&producer_threads[i], NULL, producer_thread_func, NULL);
    }
    
    for (int i = 0; i < consumers; ++i) {
        pthread_create(&consumer_threads[i], NULL, consumer_thread_func, NULL);
    }


    wait_until_producers_produced_all_numbers();
    wait_consumers_queue_empty();
    stop_consumers();

    for (int i = 0; i < producers; ++i) {
        pthread_join(producer_threads[i], NULL);
    }
    for (int i = 0; i < consumers; ++i) {
        pthread_join(consumer_threads[i], NULL);
    }

    free(producer_threads);
    free(consumer_threads);
}

  


/*
 * TODO: Implement start_consumers_producers.
 * This function should:
 *  - Print the configuration (number of consumers, producers, seed).
 *  - Seed the random number generator using srand().
 *  - Create producer and consumer threads.
 */


/*
 * TODO: Implement stop_consumers to stop all consumers threads.
 */
    // TODO: Stop the consumer thread with the given id.
    void stop_consumers() {
        atomic_store(&producers_finished, 1);
        condition_variable_broadcast(&not_empty);
    }



/*
 * TODO: Implement print_msg to perform synchronized printing.
 */


/*
 * TODO: Implement wait_until_producers_produced_all_numbers 
 * The function should wait until all numbers between 0 and 1,000,000 have been produced.
 */


/*
 * TODO: Implement wait_consumers_queue_empty to wait until queue is empty, 
 * if queue is already empty - return immediately without waiting.
 */


/*
 * TODO: Implement a main function that controls the producer-consumer process
 */
int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("usage: cp_pattern [consumers] [producers] [seed]\n");
        return 1;
    }

    int consumers = atoi(argv[1]);
    int producers = atoi(argv[2]);
    int seed = atoi(argv[3]);

    if (consumers <= 0 || producers <= 0 || seed < 0) {
        printf("usage: cp_pattern [consumers] [producers] [seed]\n");
        return 1;
    }

    start_consumers_producers(consumers, producers, seed);

    return 0;
}


