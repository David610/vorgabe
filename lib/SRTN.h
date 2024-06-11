#ifndef SRTN_H
#define SRTN_H
#include "queue.h"
#include "process.h"
#include <stdlib.h>

/**
 * Determine which is the current process to be processed. 
 * @param running_process the previously running process
 * @result a pointer to the new running process (if something changed) or to the previously running process
 */
process *SRTN_tick(process * running_process);

/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int SRTN_startup();

/**
 * Handle new arrivals
 * @param arriving_process A pointer to a new arriving process
 * @param running_process A pointer to the previously running process
 * @result A pointer to the new active (running) process (if that changed) or to the previously running process
 */
process *SRTN_new_arrival(process * arriving_process, process * running_process);

/**
 * Called once after all processes were handled. In case you want to clean up something
 */
void SRTN_finish();

/**
 * Find the process with the shortest remaining time
 * @result a pointer to the process with the shortest remaining time
 */
process *find_shortest_remaining_time_process();

#endif // SRTN_H
