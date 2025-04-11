#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_functions.h"
#include <time.h>

extern rwlock_t table_lock;

struct timespec ts2;
long long nanosec2;

hashRecord *head = NULL;

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

//delete
// need to be imporved
void check_delete(char * name) {

    //check if all of the insert statements are done from the input file
    
    //return 1 if the delete function SHOULD NOT RUN add name to a queue
    //add the names to a queue
    return; 

    //if the code should run run everything in the queue and print this line
    clock_gettime(CLOCK_REALTIME,&ts2);
    fprintf(out, "%lld%ld,DELETE AWAKENED\n", ts2.tv_sec, ts2.tv_nsec);
    delete(name); //make sure you delete everything in the queue
    return;
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
