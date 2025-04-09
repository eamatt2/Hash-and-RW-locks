#ifndef RWLOCK_H
#define RWLOCK_H

#include <semaphore.h>
#include "../chash.h"


// Reader-writer lock structure
typedef struct {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

// Reader-writer lock function declarations
void rwlock_init(rwlock_t *lock);
void rwlock_acquire_readlock(rwlock_t *lock);
void rwlock_release_readlock(rwlock_t *lock);
void rwlock_acquire_writelock(rwlock_t *lock);
void rwlock_release_writelock(rwlock_t *lock);

// Semaphore wrapper functions
void Sem_init(sem_t *sem, int value);
void Sem_wait(sem_t *sem);
void Sem_post(sem_t *sem);

#endif // RWLOCK_H