#include "../lib/SRTN.h"

static queue_object *SRTN_queue;
//You can add more global variables here

process *SRTN_tick(process *running_process)
{
    // Check if there's a running process
    if (running_process) {
        running_process->time_left--;

        // Check if the running process is finished
        if (running_process->time_left == 0) {
            queue_poll(SRTN_queue); // Remove the finished process from the queue
            return find_shortest_remaining_time_process(); // Find the next process to run
        }

        // Check if there's a process with shorter remaining time
        process *shortest_time_process = find_shortest_remaining_time_process();
        if (shortest_time_process && shortest_time_process->time_left < running_process->time_left) {
            queue_add(running_process, SRTN_queue); // Preempt the current process
            queue_poll(SRTN_queue); // Remove the shortest remaining time process from the queue
            return shortest_time_process; // Run the shortest remaining time process
        }

        return running_process; // Continue running the current process
    }

    return find_shortest_remaining_time_process(); // Find the next process to run if there's no running process

}

int SRTN_startup()
{
    // Create a new queue for SRTN processes
    SRTN_queue = new_queue();
    if (!SRTN_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    return 0; // Return 0 if everything was fine
}

process *SRTN_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        queue_add(arriving_process, SRTN_queue); // Add the new process to the queue
    }

    // Check if the arriving process has shorter remaining time than the current running process
    if (running_process) {
        process *shortest_time_process = find_shortest_remaining_time_process();
        if (shortest_time_process && shortest_time_process->time_left < running_process->time_left) {
            queue_add(running_process, SRTN_queue); // Preempt the current process
            queue_poll(SRTN_queue); // Remove the shortest remaining time process from the queue
            return shortest_time_process; // Run the shortest remaining time process
        }
    }

    return running_process;
}

void SRTN_finish()
{
    // Free all processes in the queue
    while (SRTN_queue->front) {
        process *proc = queue_poll(SRTN_queue);
        free(proc);
    }
    free_queue(SRTN_queue);
}
