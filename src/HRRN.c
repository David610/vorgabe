#include "../lib/HRRN.h"

static queue_object *HRRN_queue;
//You can add more global variables and structs here

process *HRRN_tick(process *running_process)
{
    // Update waiting times for all processes in the queue
    update_waiting_times(HRRN_queue);

    // If there's a running process, decrement its time left
    if (running_process && running_process->time_left > 0) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            return NULL; // Process is done, return NULL
        }
        return running_process; // Continue running the current process
    } else {
        // Find the process with the highest response ratio to run next
        if (!HRRN_queue->front) return NULL; // If the queue is empty, return NULL

        queue_object *current_node = HRRN_queue->front;
        queue_object *prev_node = NULL;
        HRRN_process *highest_rr_process = current_node->data;
        queue_object *highest_rr_node = current_node;

        // Find the process with the highest response ratio
        while (current_node) {
            HRRN_process *current_process = current_node->data;
            if (current_process->rr > highest_rr_process->rr) {
                highest_rr_process = current_process;
                highest_rr_node = current_node;
            }
            current_node = current_node->next;
        }

        // Remove the selected process from the queue
        if (highest_rr_node == HRRN_queue->front) {
            HRRN_queue->front = highest_rr_node->next;
        } else {
            prev_node->next = highest_rr_node->next;
        }
        if (highest_rr_node == HRRN_queue->rear) {
            HRRN_queue->rear = prev_node;
        }
        free(highest_rr_node);

        return highest_rr_process->this_process;
    }
}

int HRRN_startup()
{
    // Create a new queue for HRRN processes
    HRRN_queue = new_queue();
    if (!HRRN_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    return 0; // Return 0 if everything was fine
}

process *HRRN_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        // Create a new HRRN_process for the arriving process
        HRRN_process *new_hrrn_process = (HRRN_process *)malloc(sizeof(HRRN_process));
        new_hrrn_process->this_process = arriving_process;
        new_hrrn_process->waiting_time = 0;
        new_hrrn_process->rr = calculate_rr(new_hrrn_process);

        // Add the new HRRN_process to the queue
        queue_add(new_hrrn_process, HRRN_queue);
    }

    // Return the currently running process
    return running_process;
}

void HRRN_finish()
{
    // Free all processes in the queue
    while (HRRN_queue->front) {
        HRRN_process *hrrn_process = queue_poll(HRRN_queue);
        free(hrrn_process);
    }
    free_queue(HRRN_queue);
}
