#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_functions.h"
#include <time.h>

extern rwlock_t table_lock;
extern struct timespec ts2;

struct timespec ts2;
long long nanosec2;

hashRecord *head = NULL;

int inserts_complete = 0;

typedef struct DeleteNode {
    char name[50];
    struct DeleteNode *next;
} DeleteNode;

static DeleteNode *deleteQueueHead = NULL;
static DeleteNode *deleteQueueTail = NULL;

//insert
uint32_t insert(char *name, uint32_t salary) {
    // lock is not defined
    rwlock_acquire_writelock(&table_lock);

    uint32_t hash = jenkins_one_at_a_time_hash(name, strlen(name));
    hashRecord *curr = head;

    while(curr != NULL) {
        if(curr->hash == hash) {
            curr->salary = salary;
            return hash;
        }
        curr = curr->next;
    }

    hashRecord *new_node = (hashRecord *) malloc(sizeof(hashRecord));
    new_node->hash = hash;
    strncpy(new_node->name,name,49);
    new_node->name[49] = '\0';
    new_node->salary = salary;
    new_node->next = head;
    head = new_node;

    rwlock_release_writelock(&table_lock);
    return hash;
}

// Helper function to enqueue a deletion request
void enqueue_delete(char *name) {
    DeleteNode *node = (DeleteNode *) malloc(sizeof(DeleteNode));
    if (node == NULL) {
        perror("Failed to allocate memory for deletion queue node");
        exit(EXIT_FAILURE);
    }
    strncpy(node->name, name, 49);
    node->name[49] = '\0';
    node->next = NULL;

    if (deleteQueueTail == NULL) {
        // Queue is empty
        deleteQueueHead = node;
        deleteQueueTail = node;
    } else {
        deleteQueueTail->next = node;
        deleteQueueTail = node;
    }
}

// Helper function to process all queued deletion requests
void process_delete_queue() {
    DeleteNode *current;
    while (deleteQueueHead != NULL) {
        current = deleteQueueHead;
        deleteQueueHead = deleteQueueHead->next;
        if (deleteQueueHead == NULL)
            deleteQueueTail = NULL;
        // Invoke the existing delete function (which handles locking)
        delete(current->name);
        free(current);
    }
}


void check_delete(char *name) {
    // If the deletion queue is currently empty and inserts are still pending, log that we are waiting on insert operations
    if (!inserts_complete && deleteQueueHead == NULL) {
         clock_gettime(CLOCK_REALTIME, &ts2);
         fprintf(out, "%ld: WAITING ON INSERTS\n", ts2.tv_nsec);
    }

    // Enqueue the deletion request
    enqueue_delete(name);

    // If inserts are not complete, return without processing the queue
    if (!inserts_complete)
         return;

    // All insert commands are complete
    clock_gettime(CLOCK_REALTIME, &ts2);
    fprintf(out, "%lld%ld,DELETE AWAKENED\n", ts2.tv_sec, ts2.tv_nsec);

    // Process all pending deletions
    process_delete_queue();
}




void delete(char *name) {
    rwlock_acquire_writelock(&table_lock);

    uint32_t hash = jenkins_one_at_a_time_hash(name, strlen(name));

    hashRecord *curr = head, *prev = NULL;

    while (curr != NULL) {
        if (curr->hash == hash) {
            if (prev == NULL)
                head = curr->next;
            else
                prev->next = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    rwlock_release_writelock(&table_lock);
}

//search
hashRecord* search(char *name) {
    rwlock_acquire_readlock(&table_lock);

    uint32_t hash = jenkins_one_at_a_time_hash(name,strlen(name));
    hashRecord *curr = NULL;

    while(curr != NULL) {
        if(curr->hash == hash) {
            rwlock_release_readlock(&table_lock);
            return curr;
        }
        curr = curr->next;
    }

    rwlock_release_readlock(&table_lock);
    return NULL;

}

void printBucket(hashRecord *head) {
    hashRecord *current = head;
    printf("I am trying my best???");
    while (current != NULL) {
        fprintf(out, "Hash: %u, Name: %s, Salary: %u\n", current->hash, current->name, current->salary);
        current = current->next;
    }
}
