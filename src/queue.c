#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

queue_object *new_queue() {
    queue_object *queue = (queue_object *)malloc(sizeof(queue_object));
    if (!queue) return NULL;
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int queue_add(void *new_object, queue_object *queue) {
    queue_node *new_node = (queue_node *)malloc(sizeof(queue_node));
    if (!new_node) {
        return 1; // Return 1 if memory allocation failed
    }
    new_node->data = new_object;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    return 0; // Return 0 if the operation was successful
}

void *queue_poll(queue_object *queue) {
    if (!queue->front) return NULL;
    queue_node *front_node = queue->front;
    void *data = front_node->data;
    queue->front = front_node->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(front_node);
    queue->size--;
    return data;
}

void *queue_peek(queue_object *queue) {
    if (!queue->front) return NULL;
    return queue->front->data;
}

void free_queue(queue_object *queue) {
    while (queue->front) {
        queue_node *front_node = queue->front;
        queue->front = front_node->next;
        free(front_node);
    }
    free(queue);
}
