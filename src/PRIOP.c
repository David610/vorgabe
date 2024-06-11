#include "../lib/PRIOP.h"

// Global variable to store the priority queue
static queue_object *PRIOP_queue;

// Function to find the highest priority process
process *find_highest_priority_process() {
    if (!PRIOP_queue || !PRIOP_queue->front) return NULL;

    queue_node *current_node = PRIOP_queue->front;
    process *highest_prio_process = (process *)current_node->data;

    while (current_node) {
        process *current_process = (process *)current_node->data;
        if (current_process->priority < highest_prio_process->priority) {
            highest_prio_process = current_process;
        }
        current_node = current_node->next;
    }

    return highest_prio_process;
}

int PRIOP_startup() {
    // Initialize the priority queue
    PRIOP_queue = new_queue();
    if (!PRIOP_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    return 0; // Return 0 if everything was fine
}

process *PRIOP_tick(process *running_process) {
    if (running_process) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            queue_poll(PRIOP_queue); // Remove the finished process from the queue
            return find_highest_priority_process(); // Find the next process to run
        }
        process *highest_prio_process = find_highest_priority_process();
        if (highest_prio_process->priority < running_process->priority) {
            queue_add(running_process, PRIOP_queue); // Preempt the current process
            queue_poll(PRIOP_queue); // Remove the highest priority process from the queue
            return highest_prio_process; // Run the highest priority process
        }
        return running_process; // Continue running the current process
    }
    return find_highest_priority_process(); // Find the next process to run if there's no running process
}

process *PRIOP_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        queue_add(arriving_process, PRIOP_queue);
    }
    process *highest_prio_process = find_highest_priority_process();
    if (running_process) {
        if (highest_prio_process->priority < running_process->priority) {
            queue_add(running_process, PRIOP_queue);
            queue_poll(PRIOP_queue);
            return highest_prio_process;
        }
        return running_process;
    }
    return highest_prio_process;
}

void PRIOP_finish() {
    while (PRIOP_queue->front) {
        process *proc = (process *)queue_poll(PRIOP_queue);
        free(proc);
    }
    free_queue(PRIOP_queue);
}
