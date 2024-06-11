#include "../lib/RR.h"

static queue_object *RR_queue;
//You can add more global variables
// Global variable to store the quantum
static int RR_quantum;
// Global variable to store the current time slice
static int RR_time_slice;

process *RR_tick(process *running_process)
{
    // Check if there's a running process
    if (running_process) {
        running_process->time_left--;
        RR_time_slice++;
        
        // Check if the running process is finished
        if (running_process->time_left == 0) {
            queue_poll(RR_queue); // Remove the finished process from the queue
            RR_time_slice = 0;
            return queue_poll(RR_queue); // Find the next process to run
        }
        
        // Check if the time slice for the current process is over
        if (RR_time_slice >= RR_quantum) {
            queue_add(running_process, RR_queue); // Move the process to the back of the queue
            RR_time_slice = 0;
            return queue_poll(RR_queue); // Find the next process to run
        }
        
        return running_process; // Continue running the current process
    }
    
    return queue_poll(RR_queue); // Find the next process to run if there's no running process
}

int RR_startup(int quantum)
{
    // Create a new queue for RR processes
    RR_queue = new_queue();
    if (!RR_queue) {
        return 1; // Return 1 if queue initialization failed
    }
    
    // Set the quantum and initialize the time slice
    RR_quantum = quantum;
    RR_time_slice = 0;
    
    return 0; // Return 0 if everything was fine
}

process *RR_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process) {
        queue_add(arriving_process, RR_queue); // Add the new process to the queue
    }
    
    return running_process;
}

void RR_finish()
{
    // Free all processes in the queue
    while (RR_queue->front) {
        process *proc = queue_poll(RR_queue);
        free(proc);
    }
    free_queue(RR_queue);
}
