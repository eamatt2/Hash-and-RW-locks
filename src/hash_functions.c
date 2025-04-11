#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_functions.h"

hashRecord *head = NULL;
extern rwlock_t table_lock;

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
