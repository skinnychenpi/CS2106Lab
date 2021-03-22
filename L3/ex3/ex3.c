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
  pthread_mutex_init(&(lock->readLock),NULL);
  pthread_mutex_init(&(lock->writeMutex),NULL);
  
  pthread_mutex_lock(&(lock->readLock));
  
  lock->reader_count = 0;
  lock->writer_count = 0;
  lock->writer_wants_enter = 0;
}

void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->writeMutex));
  lock->writer_wants_enter++;
  pthread_mutex_unlock(&(lock->writeMutex));

  pthread_mutex_lock(&(lock->writeLock));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  //TODO: modify as needed
  lock->writer_count--;
  lock->writer_wants_enter--;
  pthread_mutex_unlock(&(lock->writeLock));
  pthread_mutex_unlock(&(lock->readLock));
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->writeMutex));
  if (lock->writer_wants_enter != 0) {
    pthread_mutex_lock(&(lock->readLock));
  }
  pthread_mutex_unlock(&(lock->writeMutex));

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
