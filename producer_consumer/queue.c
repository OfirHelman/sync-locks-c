#include "queue.h"
#include <stdlib.h>

// ===== Queue Functions =====
void queue_init(queue_t* q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void queue_enqueue(queue_t* q, int value) {
    node_t* new_node = malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;
    if (q->tail) {
        q->tail->next = new_node;
    } else {
        q->head = new_node;
    }
    q->tail = new_node;
    q->size++;
}

int queue_dequeue(queue_t* q, int* value) {
    if (q->head == NULL) return 0;
    node_t* temp = q->head;
    *value = temp->value;
    q->head = temp->next;
    if (q->head == NULL) q->tail = NULL;
    free(temp);
    q->size--;
    return 1;
}

int queue_is_empty(queue_t* q) {
    return q->head == NULL;
}