/*************************************
* Lab 3 Exercise 4
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

#define ensure_successful_malloc(ptr)                           \
    if (ptr == NULL) {                                          \
        printf("Memory allocation unsuccessful for" #ptr "\n"); \
        exit(1);                                                \
    }

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.
extern int num_of_cars;
extern int num_of_segments;
extern int concurrent_cars;
extern sem_t concurrent_cars_mutex;

sem_t *segmentEntries;
sem_t maximumCarsController;
pthread_mutex_t maximumCarsControllerMutex;




void initialise()
{
    //TODO: Your code here
    segmentEntries = malloc(sizeof(sem_t) * num_of_segments);
    ensure_successful_malloc(segmentEntries);
    for (int i = 0; i < num_of_segments ; i++) {
        sem_init(&segmentEntries[i], 0, 1);
    }
    // sem_init(&maximumCarsController, 0, num_of_segments - 1);
    
    // maximumCarsController = malloc(sizeof(sem_t));
    // ensure_successful_malloc(maximumCarsController);
    sem_init(&maximumCarsController, 0, 1);
    pthread_mutex_init(&maximumCarsControllerMutex,NULL);

    

}

void cleanup()
{
    //TODO: Your code here
    for (int i = 0; i < num_of_segments ; i++) {
        sem_destroy(&segmentEntries[i]);
    }
    free(segmentEntries);
    // sem_destroy(maximumCarsController);
    // free(maximumCarsController);
}

void* car(void* car)
{
    //TODO: Your code here, see the general steps below

    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    
    car_struct* theCar = ((car_struct*)car);
    int entry = theCar->entry_seg;
    int destination = theCar->exit_seg;
    
    // Enter
    sem_wait(&maximumCarsController);
    // pthread_mutex_lock(&maximumCarsControllerMutex);
    
    // sem_wait(&segmentEntries[entry]);
    printf("Car No.%d enters roundabout.\n",theCar->car_id);
    
    // enter_roundabout(theCar);
    // // Proceed
    // int current = entry;
    // while(current != destination) {
    //     int next = NEXT(current,num_of_segments);
    //     // sem_wait(&segmentEntries[next]);
    //     // sem_post(&segmentEntries[current]);
    //     move_to_next_segment(theCar);
    //     current = theCar->current_seg;
    // }
    // // Exit
    // exit_roundabout(theCar);
    // // sem_post(&segmentEntries[destination]);
    
    printf("Car No.%d exits roundabout.\n",theCar->car_id);
    
    sem_post(&maximumCarsController);
    // pthread_mutex_unlock(&maximumCarsControllerMutex);

    
    pthread_exit(NULL);
}
