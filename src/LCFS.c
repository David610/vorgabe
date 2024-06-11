#include "../lib/LCFS.h"

static queue_object *LCFS_queue;
//You can add more global variables here

int LCFS_startup()
{
    // Initialize the queue when the scheduler starts
    LCFS_queue = new_queue();
    if (!LCFS_queue) {
        // If the queue couldn't be initialized, return an error code
        return 1;
    }
    return 0;
}

process *LCFS_tick(process *running_process)
{
    // This function determines the next process to run each tick
    
    // If there's a running process and it still has time left, keep it running
    if (running_process && running_process->time_left > 0) {
        running_process->time_left--; // Decrease the time left for the running process
        return running_process; // Continue running the current process
    } else {
        // If no process is running or the current process is finished, get the next process from the queue
        return queue_poll(LCFS_queue); // Fetch and remove the next process from the queue
    }
}

process *LCFS_new_arrival(process *arriving_process, process *running_process)
{
    // This function handles new processes arriving at the scheduler
    
    // Add the new arriving process to the queue
    if (arriving_process) {
        queue_add(arriving_process, LCFS_queue);
    }
    
    // LCFS schedules the most recently arrived process, so the arriving process should be the new running process
    return arriving_process ? arriving_process : running_process;
}

void LCFS_finish()
{
    // Clean up the queue and any other resources when the scheduler finishes
    free_queue(LCFS_queue);
}
