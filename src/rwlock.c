#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rwlock.h"
#include <time.h>

//global time variables
struct timespec ts;
long long nanosec;

// Semaphore wrapper functions
void Sem_init(sem_t *sem, int value) {
    if (sem_init(sem, 0, value) != 0) {
        perror("sem_init");
        exit(1);
    }
}

void Sem_wait(sem_t *sem) {
    if (sem_wait(sem) != 0) {
        perror("sem_wait");
        exit(1);
    }
}

void Sem_post(sem_t *sem) {
    if (sem_post(sem) != 0) {
        perror("sem_post");
        exit(1);
    }
}

// Initialize the reader-writer lock
void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;
    Sem_init(&lock->lock, 1); 
    Sem_init(&lock->writelock, 1); 
}

// Acquire a read lock
void rwlock_acquire_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
        Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
    clock_gettime(CLOCK_REALTIME,&ts);
    fprintf(out, "%ld,READ LOCK AQUIRED\n", ts.tv_nsec);
}

// Release a read lock
void rwlock_release_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
        Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
    clock_gettime(CLOCK_REALTIME,&ts);
    fprintf(out, "%ld,READ LOCK RELEASED\n", ts.tv_nsec);
}

// Acquire a write lock
void rwlock_acquire_writelock(rwlock_t *lock) {
    Sem_wait(&lock->writelock);
    clock_gettime(CLOCK_REALTIME,&ts);
    fprintf(out, "%ld,WRITE LOCK AQUIRED\n", ts.tv_nsec);
    printf("Write lock awughe");
}

// Release a write lock
void rwlock_release_writelock(rwlock_t *lock) {
    Sem_post(&lock->writelock);
    clock_gettime(CLOCK_REALTIME,&ts);
    fprintf(out, "%ld,WRITE LOCK RELEASED\n", ts.tv_nsec);
}