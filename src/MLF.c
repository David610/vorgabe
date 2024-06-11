#include "../lib/MLF.h"
#include <stdio.h>
static queue_object **MLF_queues;
//You can add more global variables here
// Global variables to keep track of the current level and time slice
static int current_level = 0;
static int time_slice = 0;

process *MLF_tick(process *running_process)
{
    // Check if there's a running process
    if (running_process) {
        running_process->time_left--;
        time_slice++;
        
        // Check if the running process is finished
        if (running_process->time_left == 0) {
            queue_poll(MLF_queues[current_level]); // Remove the finished process from the queue
            time_slice = 0;
            return find_next_process(); // Find the next process to run
        }
        
        // Check if the time slice for the current level is over
        if (time_slice >= (1 << current_level)) {
            if (current_level < NUM_LEVELS - 1) {
                queue_add(queue_poll(MLF_queues[current_level]), MLF_queues[current_level + 1]); // Move the process to the next level
            } else {
                queue_add(queue_poll(MLF_queues[current_level]), MLF_queues[current_level]); // Keep the process in the same level if it's the last level
            }
            time_slice = 0;
            return find_next_process(); // Find the next process to run
        }
        
        return running_process; // Continue running the current process
    }
    
    return find_next_process(); // Find the next process to run if there's no running process
}

int MLF_startup()
{
    // Allocate memory for the queues
    MLF_queues = (queue_object **)malloc(NUM_LEVELS * sizeof(queue_object *));
    if (!MLF_queues) {
        return 1; // Return 1 if memory allocation failed
    }
    
    // Initialize each queue
    for (int i = 0; i < NUM_LEVELS; i++) {
        MLF_queues[i] = new_queue();
        if (!MLF_queues[i]) {
            return 1; // Return 1 if queue initialization failed
        }
    }
    
    return 0; // Return 0 if everything was fine
}



process *MLF_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        queue_add(arriving_process, MLF_queues[0]); // Add the new process to the highest priority queue
    }
    
    return running_process;
}

void MLF_finish()
{
    // Free all processes in the queues
    for (int i = 0; i < NUM_LEVELS; i++) {
        free_queue(MLF_queues[i]);
    }
    
    // Free the memory allocated for the queues
    free(MLF_queues);
}
