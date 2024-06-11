#include "../lib/SRTN.h"

static queue_object *SRTN_queue;

// Function to find the process with the shortest remaining time
process *find_shortest_remaining_time_process() {
    if (!SRTN_queue || !SRTN_queue->front) return NULL;

    queue_node *current_node = SRTN_queue->front;
    process *shortest_time_process = (process *)current_node->data;

    while (current_node) {
        process *current_process = (process *)current_node->data;
        if (current_process->time_left < shortest_time_process->time_left) {
            shortest_time_process = current_process;
        }
        current_node = current_node->next;
    }

    return shortest_time_process;
}

// Initialize the SRTN scheduler
int SRTN_startup() {
    SRTN_queue = new_queue();
    if (!SRTN_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    return 0; // Return 0 if everything was fine
}

// Handle new arriving processes
process *SRTN_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        queue_add(arriving_process, SRTN_queue); // Add the new process to the queue
    }

    if (running_process) {
        process *shortest_time_process = find_shortest_remaining_time_process();
        if (shortest_time_process && shortest_time_process->time_left < running_process->time_left) {
            queue_add(running_process, SRTN_queue); // Preempt the current process
            queue_poll(SRTN_queue); // Remove the shortest time process from the queue
            return shortest_time_process; // Run the shortest time process
        }
        return running_process; // Continue running the current process
    }

    return find_shortest_remaining_time_process(); // Find the next process to run if there's no running process
}

// Handle the ticking of the scheduler
process *SRTN_tick(process *running_process) {
    if (running_process) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            queue_poll(SRTN_queue); // Remove the finished process from the queue
            return find_shortest_remaining_time_process(); // Find the next process to run
        }
        process *shortest_time_process = find_shortest_remaining_time_process();
        if (shortest_time_process && shortest_time_process->time_left < running_process->time_left) {
            queue_add(running_process, SRTN_queue); // Preempt the current process
            queue_poll(SRTN_queue); // Remove the shortest time process from the queue
            return shortest_time_process; // Run the shortest time process
        }
        return running_process; // Continue running the current process
    }

    return find_shortest_remaining_time_process(); // Find the next process to run if there's no running process
}

// Clean up the SRTN scheduler
void SRTN_finish() {
    while (SRTN_queue->front) {
        process *proc = (process *)queue_poll(SRTN_queue);
        free(proc);
    }
    free_queue(SRTN_queue);
}
