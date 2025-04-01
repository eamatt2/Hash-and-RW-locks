#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rwlock.h"

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
}

// Release a read lock
void rwlock_release_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
        Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

// Acquire a write lock
void rwlock_acquire_writelock(rwlock_t *lock) {
    Sem_wait(&lock->writelock);
}

// Release a write lock
void rwlock_release_writelock(rwlock_t *lock) {
    Sem_post(&lock->writelock);
}