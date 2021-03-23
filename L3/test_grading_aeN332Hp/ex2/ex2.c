/*************************************
* Lab 3 Exercise 2
* Name: Chen Yuheng
* Student Id: A0229929L
* Lab Group: SOLO
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->writeLock),NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->writeLock));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  //TODO: modify as needed
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->writeLock));
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  if (lock->reader_count == 0) {
    pthread_mutex_lock(&(lock->writeLock));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  if (lock->reader_count == 0) {
    pthread_mutex_unlock(&(lock->writeLock));
  }
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_destroy(&(lock->mutex));
  pthread_mutex_destroy(&(lock->writeLock));
}