#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stddef.h>

typedef struct hash_struct
{
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

uint32_t jenkins_one_at_a_time_hash(const char* key, size_t length);

#endif 
