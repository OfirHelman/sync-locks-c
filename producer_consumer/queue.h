


typedef struct node {
    int value;
    struct node* next;
} node_t;

typedef struct queue{
    node_t* head;
    node_t* tail;
    int size;
} queue_t;

/** Queue utility functions **/
void queue_init(queue_t* q);
void queue_enqueue(queue_t* q, int value);
int queue_dequeue(queue_t* q, int* value);
int queue_is_empty(queue_t* q);