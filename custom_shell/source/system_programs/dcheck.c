#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fptr = fopen("dspawn.log", "r");
    if (!fptr) {
        perror("Failed to open dspawn.log");
        return 1;
    }

    char line[256];
    char path[64];
    int pid;
    int counter = 0;
    int seen_pids[512];
    int seen_count = 0;  



    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "Daemon PID %d", &pid) == 1) {
            // check for unique pid
            int already_seen = 0;
            for (int i = 0; i < seen_count; i++) {
                if (seen_pids[i] == pid) {
                    already_seen = 1;
                    break;
                    }
            }
            if (already_seen) {
                continue;  // skip this line, since PID was already processed
            }
            seen_pids[seen_count++] = pid;

            
            snprintf(path, sizeof(path), "/proc/%d/status", pid);  // check if pid is still live
            FILE *status_file = fopen(path, "r");
            if (status_file) {  // if file is openable means it is still live
                counter++;
                fclose(status_file);
            }
        }
    }

    fclose(fptr);

    if (counter == 0) {
        printf("No daemon is alive right now.\n");
    } else {
        printf("Live daemons: %d\n", counter);
    }

    return 0;
}
