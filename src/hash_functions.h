#include "hash.h"
#include "rwlock.h"

uint32_t insert(char *name, uint32_t salary);
void delete(char *name);
hashRecord* search(char *name);
void printBucket(hashRecord *head);
void check_delete(char * name);
extern hashRecord *head;