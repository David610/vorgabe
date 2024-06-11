#include "../lib/HRRN.h"

// Global variable to store the queue of processes
static queue_object *HRRN_queue;

// Function to update the waiting times of processes
void update_waiting_times(queue_object *queue) {
    queue_node *current_node = queue->front;
    while (current_node) {
        HRRN_process *hrrn_process = (HRRN_process *)current_node->data;
        hrrn_process->waiting_time++;
        current_node = current_node->next;
    }
}

// Function to calculate the response ratio of a process
float calculate_rr(HRRN_process *hrrn_process) {
    return (float)(hrrn_process->waiting_time + hrrn_process->this_process->time_left) / hrrn_process->this_process->time_left;
}

// Initialize the HRRN scheduler
int HRRN_startup() {
    // Create a new queue for HRRN processes
    HRRN_queue = new_queue();
    if (!HRRN_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    return 0; // Return 0 if everything was fine
}

// Function to find the process with the highest response ratio
static HRRN_process* find_highest_rr_process() {
    if (!HRRN_queue || !HRRN_queue->front) return NULL;

    queue_node *current_node = HRRN_queue->front;
    HRRN_process *highest_rr_process = (HRRN_process *)current_node->data;
    float highest_rr = calculate_rr(highest_rr_process);

    while (current_node) {
        HRRN_process *current_process = (HRRN_process *)current_node->data;
        float current_rr = calculate_rr(current_process);
        if (current_rr > highest_rr) {
            highest_rr = current_rr;
            highest_rr_process = current_process;
        }
        current_node = current_node->next;
    }

    return highest_rr_process;
}

// Handle the ticking of the scheduler
process *HRRN_tick(process *running_process) {
    // Update waiting times for all processes in the queue
    update_waiting_times(HRRN_queue);

    // If there's a running process, decrement its time left
    if (running_process) {
        running_process->time_left--;
        if (running_process->time_left == 0) {
            queue_poll(HRRN_queue); // Remove the finished process from the queue
            HRRN_process *highest_rr_process = find_highest_rr_process(); // Find the next process to run
            return highest_rr_process ? highest_rr_process->this_process : NULL;
        }

        // Find the process with the highest response ratio to run next
        HRRN_process *highest_rr_process = find_highest_rr_process();
        if (highest_rr_process && highest_rr_process->this_process->time_left < running_process->time_left) {
            queue_add(running_process, HRRN_queue); // Preempt the current process
            queue_poll(HRRN_queue); // Remove the highest response ratio process from the queue
            return highest_rr_process->this_process; // Run the highest response ratio process
        }

        return running_process; // Continue running the current process
    }

    HRRN_process *highest_rr_process = find_highest_rr_process(); // Find the next process to run if there's no running process
    return highest_rr_process ? highest_rr_process->this_process : NULL;
}

// Handle new arriving processes
process *HRRN_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        // Create a new HRRN_process for the arriving process
        HRRN_process *new_hrrn_process = (HRRN_process *)malloc(sizeof(HRRN_process));
        new_hrrn_process->this_process = arriving_process;
        new_hrrn_process->waiting_time = 0;
        new_hrrn_process->rr = calculate_rr(new_hrrn_process);

        // Add the new HRRN_process to the queue
        queue_add(new_hrrn_process, HRRN_queue);
    }

    // Check if the arriving process has higher priority than the current running process
    if (running_process) {
        HRRN_process *highest_rr_process = find_highest_rr_process();
        if (highest_rr_process && highest_rr_process->this_process->time_left < running_process->time_left) {
            queue_add(running_process, HRRN_queue); // Preempt the current process
            queue_poll(HRRN_queue); // Remove the highest response ratio process from the queue
            return highest_rr_process->this_process; // Run the highest response ratio process
        }
    }

    return running_process;
}

// Clean up the HRRN scheduler
void HRRN_finish() {
    // Free all processes in the queue
    while (HRRN_queue->front) {
        HRRN_process *hrrn_process = (HRRN_process *)queue_poll(HRRN_queue);
        free(hrrn_process);
    }
    free_queue(HRRN_queue);
}
