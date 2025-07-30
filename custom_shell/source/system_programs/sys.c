/*Need to include info about the OS, kernel, total memory size,
currently logged-in and CPU */

/*Since in system_programs, The makefile will 
automatically detect this new script and compile it 
into [PROJECT_DIR]/bin */

#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

int main(){
    //To store system info
    struct utsname sysinfo;
    //Get system info to fill details about your computer
    if (uname(&sysinfo) == -1) {
        perror("uname failed");
        return 1;
    }

    // Get current username
    char *username = getlogin();
    if (username == NULL) {
        struct passwd *pw = getpwuid(getuid());
        if (pw != NULL) {
            username = (*pw).pw_name;
        }
    }
    if (username != NULL) {
        printf("User: %s\n", username);
    }else{
        printf("User: (not found)\n");
    }

    // Print system info
    printf("=== Basic System Info ===\n");
    printf("OS: %s\n", sysinfo.sysname);
    printf("Host: %s\n", sysinfo.nodename);
    printf("Kernel: %s\n", sysinfo.release);
    printf("User: %s\n", username);

    // Uptime (print raw output from uptime command)
    printf("\n=== Uptime ===\n");
    system("uptime -p");

    // CPU Info
    printf("\n=== CPU Info ===\n");
    system("grep 'model name' /proc/cpuinfo | head -1");

    // Memory Info
    printf("\n=== Memory Info ===\n");
    system("grep 'MemTotal' /proc/meminfo");

    // Shell
    printf("\n=== Shell ===\n");
    system("echo $SHELL");

    return 0; //program ends successfully
}