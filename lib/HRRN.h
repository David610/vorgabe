#ifndef HRRN_H
#define HRRN_H
#include "queue.h"
#include "process.h"
#include <stdlib.h>

typedef struct _HRRN_process {
    process *this_process;
    unsigned int waiting_time;
    float rr;  // response ratio
} HRRN_process;

/**
 * Determine which is the current process to be processed. 
 * @param running_process The previously running process.
 * @result A pointer to the new running process (if something changed) or to the previously running process.
 */
process *HRRN_tick(process * running_process);

/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int HRRN_startup();

/**
 * Handle new arrivals.
 * @param arriving_process A pointer to a new arriving process.
 * @param running_process A pointer to the previously running process.
 * @result A pointer to the new active (running) process (if that changed) or to the previously running process.
 */
process *HRRN_new_arrival(process * arriving_process,
                          process * running_process);

/**
 * Called once after all processes were handled. In case you want to clean up something.
 */
void HRRN_finish();

/**
 * Function to update the waiting times of processes.
 */
void update_waiting_times(queue_object *queue);

/**
 * Function to calculate the response ratio of a process.
 */
float calculate_rr(HRRN_process *hrrn_process);

#endif // HRRN_H
