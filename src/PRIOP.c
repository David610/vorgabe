#include "../lib/PRIOP.h"
#include <stdio.h>

static queue_object *PRIOP_queue;
//You can add more global variables here

process *PRIOP_tick(process *running_process)
{
    // Check if there's a running process
    if (running_process) {
        running_process->time_left--;

        // Check if the running process is finished
        if (running_process->time_left == 0) {
            // Remove the finished process from the queue
            queue_poll(PRIOP_queue);
            return find_highest_priority_process(); // Find the next process to run
        }

        // Check if there's a process with higher priority
        process *highest_prio_process = find_highest_priority_process();
        if (highest_prio_process && highest_prio_process->priority < running_process->priority) {
            queue_add(running_process, PRIOP_queue); // Preempt the current process
            queue_poll(PRIOP_queue); // Remove the highest priority process from the queue
            return highest_prio_process; // Run the highest priority process
        }

        return running_process; // Continue running the current process
    }

    return find_highest_priority_process(); // Find the next process to run if there's no running process
}

int PRIOP_startup()
{
    // Create a new queue for PRIOP processes
    PRIOP_queue = new_queue();
    if (!PRIOP_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    return 0; // Return 0 if everything was fine
}

process *PRIOP_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        queue_add(arriving_process, PRIOP_queue); // Add the new process to the queue
    }

    // Check if the arriving process has higher priority than the current running process
    if (running_process) {
        process *highest_prio_process = find_highest_priority_process();
        if (highest_prio_process && highest_prio_process->priority < running_process->priority) {
            queue_add(running_process, PRIOP_queue); // Preempt the current process
            queue_poll(PRIOP_queue); // Remove the highest priority process from the queue
            return highest_prio_process; // Run the highest priority process
        }
    }

    return running_process;
}

void PRIOP_finish()
{
    // Free all processes in the queue
    while (PRIOP_queue->front) {
        process *proc = queue_poll(PRIOP_queue);
        free(proc);
    }
    free_queue(PRIOP_queue);
}
