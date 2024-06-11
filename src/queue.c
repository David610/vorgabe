#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void *new_object, queue_object *queue)
{
	node *new_node = (node *)malloc(sizeof(node));
    if (!new_node) {
        return 1; // Memory allocation failed
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
    return 0;
}

void *queue_poll(queue_object *queue)
{
	if (!queue->front) {
		return NULL; // Queue is empty
    }

    node *front_node = queue->front;
    void *data = front_node->data;
    queue->front = front_node->next;

    if (!queue->front) {
        queue->rear = NULL;
    }

    free(front_node);
    queue->size--;
    return data;
}

queue_object *new_queue()
{
	queue_object *queue = (queue_object *)malloc(sizeof(queue_object));
    if (!queue) {
        return NULL; // Memory allocation failed
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
	return queue;
}

void free_queue(queue_object *queue)
{
    while (queue->front) {
        node *current = queue->front;
        queue->front = queue->front->next;
        free(current);
    }
    free(queue);
}

void *queue_peek(queue_object *queue)
{
    if (!queue->front) {
        return NULL; // Queue is empty
    }
    return queue->front->data;
}
