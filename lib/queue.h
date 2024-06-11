#ifndef QUEUE_H
#define QUEUE_H

typedef struct _queue_node {
    void *data;
    struct _queue_node *next;
} queue_node;

typedef struct _queue_object {
    queue_node *front;
    queue_node *rear;
} queue_object;

/**
 * Initialize a new queue
 */
queue_object *new_queue();

/**
 * Add an element to the queue
 */
void queue_add(void *new_object, queue_object *queue);

/**
 * Remove and return the front element from the queue
 */
void *queue_poll(queue_object *queue);

/**
 * Peek at the front element of the queue without removing it
 */
void *queue_peek(queue_object *queue);

/**
 * Free the memory allocated for the queue
 */
void free_queue(queue_object *queue);

#endif // QUEUE_H
