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

//Inserts complete global
extern int inserts_complete;

// Helper to trim newlines (optional)
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

int check_inserts(int total, int counter) { //takes the total number of inserts and checks the amnt of inserts done so far
    if(counter == total)
        return 1;
    return 0;
}

int main() {
    // Open commands.txt for reading
    FILE *cmds = fopen("commands.txt", "r");
    if (!cmds) {
        perror("Could not open commands.txt");
        return 1;
    }
    
    char line1[1024];
    int total = 0;

    while (fgets(line1, sizeof(line1), cmds)) {
        char *ptr = line1;
        while ((ptr = strstr(ptr, "insert")) != NULL) {
            total++;
            ptr += strlen("insert"); // move past the match
        }
    }
    fclose(cmds);

    cmds = fopen("commands.txt", "r");
    if (!cmds) {
        perror("Could not open commands.txt");
        return 1;
    }

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
    

    int counter = 0;

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
                insert(name, salary);
                counter++;
                inserts_complete = check_inserts(total, counter);
                check_delete(""); 
            }
        } else if (strcmp(cmd, "delete") == 0) {
            char *name = strtok(NULL, ",");
            if (name) {
                check_delete(name);
            }
        } else if (strcmp(cmd, "search") == 0) {
            char *name = strtok(NULL, ",");
            if (name) {
                search(name);
            }
        }
    }

    inserts_complete = 1;
    check_delete("");

    fprintf(out, "Finished all threads.\nNumber of lock acquisitions: %d\n", lockAquired);
    fprintf(out, "Number of lock releases: %d\n", lockReleased);
    
    printBucket(head);

    fclose(cmds);
    fclose(out);
    return 0;
}
