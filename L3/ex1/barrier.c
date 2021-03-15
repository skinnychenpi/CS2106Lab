/*************************************
* Lab 3 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "barrier.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    //TODO: Implement the function
    barrier->count = count;
    barrier->arrived = 0;
    barrier->mutex = malloc(sizeof(sem_t));
    sem_init(&(barrier->mutex), 0, 1 );
    barrier->waitQ = malloc(sizeof(sem_t));
    sem_init(&(barrier->waitQ), 0, 0 );
}

void barrier_wait ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_wait(&(barrier->mutex));
    barrier->arrived++;
    sem_post(&(barrier->mutex));
    if (barrier->arrived == barrier->count) {
        sem_post(&barrier->waitQ);
    } 
    sem_wait(&(barrier->waitQ));
    sem_post(&(barrier->waitQ));

}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_destroy(&(barrier->mutex));
    sem_destroy(&(barrier->waitQ));
    free(&(barrier->mutex));
    free(&(barrier->waitQ));
}
