#include "../lib/MLF.h"
#include <stdio.h>

// Number of levels in the MLF scheduler
#define NUM_LEVELS 4

static queue_object **MLF_queues;
static int *time_quantum;
static int current_level;
static int current_quantum;

// Function to find the next process to run
process *find_next_process() {
    for (int i = 0; i < NUM_LEVELS; i++) {
        if (MLF_queues[i]->front != NULL) {
            current_level = i;
            current_quantum = time_quantum[i];
            return (process *)MLF_queues[i]->front->data;
        }
    }
    return NULL;
}

// Initialize the MLF scheduler
int MLF_startup() {
    MLF_queues = (queue_object **)malloc(NUM_LEVELS * sizeof(queue_object *));
    if (!MLF_queues) {
        return 1; // Return 1 if allocation failed
    }

    time_quantum = (int *)malloc(NUM_LEVELS * sizeof(int));
    if (!time_quantum) {
        free(MLF_queues);
        return 1; // Return 1 if allocation failed
    }

    for (int i = 0; i < NUM_LEVELS; i++) {
        MLF_queues[i] = new_queue();
        time_quantum[i] = 1 << i; // Setting the time quantum to 2^i
    }
    return 0;
}

// Handle new arriving processes
process *MLF_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        queue_add(arriving_process, MLF_queues[0]); // Add to the highest priority queue
    }

    // Check if the currently running process should be preempted
    if (running_process) {
        current_quantum--;
        if (current_quantum <= 0) {
            int next_level = current_level < NUM_LEVELS - 1 ? current_level + 1 : current_level;
            queue_add(running_process, MLF_queues[next_level]); // Demote the process to the next level
            return find_next_process(); // Find the next process to run
        }
        return running_process; // Continue running the current process
    }

    return find_next_process(); // Find the next process to run if there's no running process
}

// Handle the ticking of the scheduler
process *MLF_tick(process *running_process) {
    // If there's a running process, decrement its time left
    if (running_process) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            queue_poll(MLF_queues[current_level]); // Remove the finished process from the queue
            return find_next_process(); // Find the next process to run
        }

        current_quantum--;
        if (current_quantum <= 0) {
            int next_level = current_level < NUM_LEVELS - 1 ? current_level + 1 : current_level;
            queue_add(running_process, MLF_queues[next_level]); // Demote the process to the next level
            return find_next_process(); // Find the next process to run
        }
        return running_process; // Continue running the current process
    }

    return find_next_process(); // Find the next process to run if there's no running process
}

// Clean up the MLF scheduler
void MLF_finish() {
    for (int i = 0; i < NUM_LEVELS; i++) {
        while (MLF_queues[i]->front) {
            process *proc = (process *)queue_poll(MLF_queues[i]);
            free(proc);
        }
        free_queue(MLF_queues[i]);
    }
    free(MLF_queues);
}
