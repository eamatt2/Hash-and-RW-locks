#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "src/hash.h"
#include "src/rwlock.h"
#include "src/hash_functions.h"
#include "chash.h"
#include <time.h>

// Global lock
rwlock_t table_lock;

//Time vars
struct timespec ts;
long long nanosec;

// Output file pointer
FILE * out = NULL;

// Helper to trim newlines (optional)
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

int main() {
    // Open commands.txt for reading
    FILE *cmds = fopen("commands.txt", "r");
    if (!cmds) {
        perror("Could not open commands.txt");
        return 1;
    }
    printf("imalive");

    // Open output.txt for writing
    out = fopen("output.txt", "w");
    if (!out) {
        perror("Could not open output.txt");
        fclose(cmds);
        return 1;
    }


    rwlock_init(&table_lock);  // Initialize lock
    char buffer[50]; //holds first line
    int dummy = 0;
    char dummys[2];
    int num_threads = 0;

    if(fgets(buffer, sizeof(buffer), cmds)) {
        if(sscanf(buffer,"%49[^,],%d,%d", dummys, &num_threads, &dummy))
            fprintf(out, "Running %d threads\n", num_threads);
    }
    

        

    char line[128];
    while (fgets(line, sizeof(line), cmds)) {
        trim_newline(line);
        char *cmd = strtok(line, ",");
        if (!cmd) continue;

        if (strcmp(cmd, "insert") == 0) {
            char *name = strtok(NULL, ",");
            char *salary_str = strtok(NULL, ",");
            if (name && salary_str) {
                uint32_t salary = atoi(salary_str);
                uint32_t hash_value = insert(name, salary);
                clock_gettime(CLOCK_REALTIME,&ts);
                fprintf(out, "%ld: INSERT,%u,%s,%s\n", ts.tv_nsec, hash_value, name, salary_str);            
            }
        } else if (strcmp(cmd, "delete") == 0) {
            char *name = strtok(NULL, ",");
            if (name) {
                delete(name);
                fprintf(out, "%ld: DELETE,%s\n", ts.tv_nsec, name);            
            }
        } else if (strcmp(cmd, "search") == 0) {
            char *name = strtok(NULL, ",");
            if (name) {
                hashRecord *res = search(name);
                if (res) {
                    fprintf(out, "SEARCH:%s %u\n", res->name, res->salary);
                } else {
                    fprintf(out, "SEARCH: NOT FOUND NOT FOUND\n");
                }
            }
        }
    }

    fprintf(out, "Number of lock acquisitions: %d\n", lockAquired);
    fprintf(out, "Number of lock releases: %d\n", lockReleased);
    
    printBucket(head);

    fclose(cmds);
    fclose(out);
    return 0;
}
