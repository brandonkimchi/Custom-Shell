#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


int main(){
    char *targetVar = getenv("BACKUP_DIR");
    if (!targetVar){
        printf("Error: BACKUP_DIR environment variable not set");
        return 1;
    }
    else{
        // formatting of time
        time_t now = time(NULL); // to find out the current time
        struct tm *t = localtime(&now);  // to give the structure in local time according to our time zone
        char timestamp[64];  // declare an output buffer of 64 bytes for our timestamp - a formatted string version of our time
        strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", t);  // formats into string
        // Year, month, day, hour, min, sec
        // strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr);

        // formatting of zip file name + timestamp
        char zipname[128]; // string buffer to stored the zipped file
        snprintf(zipname, sizeof(zipname), "./archive/backup-%s.zip", timestamp);

        // zipping 
        char command[PATH_MAX + 128]; // to store the full shell command str
        snprintf(command, sizeof(command), "zip -r %s %s", zipname, targetVar);
        // zip -r recursively zips a directory, one by one, can also zip indiv files

        printf("Creating backup of '%s' at '%s'\n", targetVar, zipname);

        // to see if successful or not 
        int outcome = system(command);
        // system() outputs 0 on success and non-zero on failure
        if(outcome == 0){
            printf("Backup created successfully");
        }
        else{
            printf("Backup failed");
        }
    }
}