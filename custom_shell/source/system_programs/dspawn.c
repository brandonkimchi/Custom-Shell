/*The dspawn system program summons a daemon process and then terminates so that the shell may 
continue to display the next prompt. This is unlike other programs where the shell waits for 
it to finish before printing the next prompt.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

char output_file_path[PATH_MAX];

static int daemon_work() {
    FILE *fptr;
    int num = 0;

    while (num < 10) {
        sleep(10);  // wait for 10 seconds

        fptr = fopen(output_file_path, "a"); //open log file
        if (fptr == NULL) {
            return EXIT_FAILURE;
        }
        fprintf(fptr, "Daemon PID %d writing line %d to the file.\n", getpid(), num); //has to match dcheck

        fclose(fptr);
        num++;
    }
    return EXIT_SUCCESS;
}


int main() {
    pid_t pid;

    // Setup log file path
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL) {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }
    strcat(output_file_path, "/dspawn.log");

    // step 1: first fork --> ensures that the process is not a process group leader 
    pid = fork();
    if (pid < 0) {
        perror("First fork failed");
        return EXIT_FAILURE;
    }
    if (pid > 0) {
        // parent exits --> step 2: exit dspawn
        return EXIT_SUCCESS;
        //shell will return while the intermedite process proceed to spawn the daemon
    }

    // step 3: setsid
    // step 1 ensures setsid() is effective
    if (setsid() < 0) {
        perror("setsid failed");
        return EXIT_FAILURE;
    }

    // step 4: ignore SIGCHLD and SIGHUP
    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // step 5: second fork
    pid = fork();
    if (pid < 0) {
        perror("Second fork failed");
        return EXIT_FAILURE;
    }
    if (pid > 0) {
        // Intermediate parent exits --> ensures that the child process is not a session leader
        // When your program becomes a daemon, you want it to
        // -Run in the background
        // -Never interact w a terminal
        // -Not affected by terminal events
        // But if daemon is a session leader, it might get accidentally assigned a controlling terminal again
        // if it opens a terminal device (e.g. /dev/tty)
        return EXIT_SUCCESS;
    }

    // step 6: set file permissions, umask(0)
    // Set file permissions to 0777 (everyone can read, write, execute)
    //Read, Write, Execute --> 111 in binary --> 7 in octal 
    // 0777 = Owner: 7 (rwx), Group: 7 (rwx), Others: 7 (rwx)
    // so we umask(0) to not take away any permissions when files are created as we want files created by the daemon to be accessed by anyone
    umask(0);

    // step 7: change working directory to root, chdir
    if (chdir("/") < 0) {
        perror("chdir failed");
        return EXIT_FAILURE;
    }

    // step 8: handle fd 0,1,2 and close all unused fds
    for (int fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
        close(fd);
    }

    // Attach file descriptors 0,1,2 (stdin, stdout, stderr) to /dev/null
    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0);
    int fd2 = dup(0);

    // step 9: execute daemon_work()
    return daemon_work();
}


