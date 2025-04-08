#include "hash.h"
#include "rwlock.h"

void insert(char *name, uint32_t salary);
void delete(char *name);
hashRecord* search(char *name);
