// This code was written by ChatGPT4
// Modify it for your own usage to implement features for PA1
// Process .cseshellrc on startup, then enter interactive loop.

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h> // for displaying time
#include <sys/resource.h>  // for displaying cpu usage

#define MAX_LINE_LENGTH 1024

extern char **environ;
char prompt_style_code[16] = "";  

const char *builtin_commands[] = {
  "cd",
  "help",
  "exit",
  "usage",
  "env",
  "setenv",
  "unsetenv"
};

int (*builtin_command_func[])(char **) = {
  &shell_cd,
  &shell_help,
  &shell_exit,
  &shell_usage,
  &list_env,
  &set_env_var,
  &unset_env_var
};

int num_builtin_functions() {
  return sizeof(builtin_commands) / sizeof(char *);
}

// Function to read a command from the user input
void read_command(char **cmd) {
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is
  // reached
  for (;;) {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n') {
      printf("\033[0m\n"); //reset colour, move to next line
      break;
    }
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE) {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1){
    return;
  }

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL) {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++) {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt() {
  char cwd[PATH_MAX];      // for current working directory
  char time_str[64];       // for formatted time string


  /*// Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time) {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }*/
  //just print prompt, don't clear screen as we want rc output still
  //fflush(stdout); // Flush the output buffer
  //printf("$$ ");  // Print the shell prompt

  // get current username
  char *username = getlogin();
  if (username == NULL) {
    struct passwd *pw = getpwuid(getuid());
    if (pw != NULL) {
      username = (*pw).pw_name;
    }
  }

  /*if (username != NULL) {
    printf("User: %s | ", username);
  }else{
    printf("User: (not found)");
  }*/

  // get current working directory
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd failed");
    return;
  }


  // formatting of time
  time_t now = time(NULL); // to find out the current time
  struct tm *t = localtime(&now);  // to give the structure in local time according to our time zone
  char timestamp[64];  // declare an output buffer of 64 bytes for our timestamp - a formatted string version of our time
  strftime(timestamp, sizeof(timestamp), "%H:%M", t);  // formats into string
  // Year, month, day, hour, min, sec
  // strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr);
  //printf("Time: %s  ", timestamp);

  // Print shell prompt with current user, current path and time
  printf("\033[38;2;255;92;205mUser: %s\033[0m ⋆.ೃ࿔*:･ | \033[38;2;200;0;255mPath: %s\033[0m | \033[38;2;0;255;255mTime: %s\033[0m ", username, cwd, timestamp);
  
  //generate random color for user input
  int r = rand() % 256;
  int g = rand() % 256;
  int b = rand() % 256;

  //sets random colour for user prompt
  printf("%s\033[38;2;%d;%d;%dm$$ ", prompt_style_code, r, g, b);


  fflush(stdout);

}

void process_rc_file() {
  char cwd[PATH_MAX];
  char rc_path[PATH_MAX + 32];
  if (!getcwd(cwd, sizeof(cwd))) return;
  snprintf(rc_path, sizeof(rc_path), "%s/.cseshellrc", cwd);

  FILE *f = fopen(rc_path, "r");
  if (!f) return;

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    if (strncmp(line, "PATH=", 5) == 0) {
      setenv("PATH", line + 5, 1);
      printf("[.cseshellrc] PATH set to: %s\n", getenv("PATH"));
      continue;
    }
    if (line[0] == '\0') continue;

    char *rc_cmd[MAX_ARGS] = {NULL};
    int i = 0;
    char *tok = strtok(line, " ");
    while (tok && i < MAX_ARGS - 1) {
      rc_cmd[i++] = strdup(tok);
      tok = strtok(NULL, " ");
    }
    rc_cmd[i] = NULL;
    if (!rc_cmd[0]) continue;

    printf("[.cseshellrc] running: %s\n", rc_cmd[0]);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == 0) {
      execvp(rc_cmd[0], rc_cmd);
      fprintf(stderr, "cseshell: command not found: %s\n", rc_cmd[0]);
      _exit(1);
    } else if (pid > 0) {
      int st;
      waitpid(pid, &st, 0);
    }
    for (int j = 0; rc_cmd[j]; j++) free(rc_cmd[j]);
  }
  fclose(f);
}

// for cpu usage display
void getCPUusage(){
  struct rusage usage;   // holds resource usage stats for a process to get info about cpu time, memory usage etc
  if (getrusage(RUSAGE_CHILDREN, &usage) == 0) {   // getrusage() calls to retrieve resource usage stats 
    // RUSAGE_CHILDREN -> stats for child processes that were terminated and waitpid
    // == 0 means successful
    double user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6;  
    // calculates total user time in seconds(ru_utime.tv_sec) + remaining time(ru_utime.tv_usec) in microseconds (/1e6)
    double sys_time = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6;
    // calculates total time spent in kernel mode in seconds + remaining time in microsec

    // Display CPU usage
    printf("\033[90m CPU time used: %.3f sec (user), %.3f sec (system)\033[0m\n", user_time, sys_time);

    // Display peak memory usage (in kilobytes)
    printf("\033[90m Peak memory usage: %ld KB\033[0m\n", usage.ru_maxrss); 
    // ru_maxrss -> peak memory usage

    // Display disk I/O
    printf("\033[90m Disk I/O: %ld block reads, %ld block writes\033[0m\n", usage.ru_inblock, usage.ru_oublock);
    // ru_inblock -> no. of block input operations
    // ru_outblock -> no. of block output operations
  }
}

//char prompt_style_code[16] = "";  // default/normal style, prompt_style_code is currently empty

int changeTextStyle(char **cmd) {
  // Handle style change command
  if (strcmp(cmd[0], "style") == 0 && cmd[1]) {    // checks for user input "style"
    if (strcmp(cmd[1], "bold") == 0) {     // checks for the exact style requested by user
        strcpy(prompt_style_code, "\033[1m");   // strcpy copies in the code into prompt_style_code 
        // [1m is the code for bold
        printf("Prompt style set to bold.\n");
        return 1;
    } else if (strcmp(cmd[1], "dim") == 0) {
        strcpy(prompt_style_code, "\033[2m");
        printf("Prompt style set to dim.\n");
        return 1;
    } else if (strcmp(cmd[1], "normal") == 0) {
        strcpy(prompt_style_code, "");   // revert back
        printf("Prompt style set to normal.\n");
        return 1;
    } else {
        printf("Unknown style: %s. Use 'bold', 'dim', or 'normal'.\n", cmd[1]);
        return 1;
    }
    
  } return 0;

}

// The main function where the shell's execution begins
int main(void) {
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS] = {NULL};  // initializes all to NULL safely
  int child_status;
  pid_t pid;
  int child_exit_status;

  // Clear screen once, then run rc so output stays
  system("clear");
  srand(time(NULL)); //seed the random number generator

  process_rc_file();
  
  while(1){
    type_prompt(); // Display the prompt
    // Free the allocated memory for the command arguments before exiting
    for (int i = 0; cmd[i] != NULL; i++){
      free(cmd[i]); // frees the memory, but old addr is still there
      cmd[i] = NULL; // need to set to NULL to forget the old memory
      } 

    read_command(cmd); // Read a command from the user

    if(cmd[0] == NULL){  // if command is empty (e.g user clicks enter)
      continue;  
    } 

  // If the command is "exit", break out of the loop to terminate the shell
    if (strcmp(cmd[0], "exit") == 0){
      //break;
      return 0;
    }

    if (changeTextStyle(cmd)) {
      continue;  
    }


    int handled = 0;
    // Loop through our command list and check if the commands exist in the builtin command list
    for (int command_index = 0; command_index < num_builtin_functions(); command_index++){
      if (strcmp(cmd[0], builtin_commands[command_index]) == 0){ // Assume args[0] contains the first word of the command
      // We will create new process to run the function with the specific command except for builtin commands.
      // These have to be done by the shell process. 
        (*builtin_command_func[command_index])(cmd);
        handled = 1;
        break;
      }
    }

    if (handled){  
      continue;
    }

    

    pid = fork();

    if (pid < 0 ){
      // Fork failed
      printf("Failed to fork");
      exit(1); // quit the shell and print the error
    }
    else if (pid == 0){
      /*char full_path[PATH_MAX];
      char cwd[1024];
      // Formulate the full path of the command to be executed
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
        execv(full_path, cmd);
        printf("Command %s not found\n", cmd[0]);
        exit(1); */
      //}
      //else {
        //printf("Failed to get current working directory.");
        //exit(0);
      execvp(cmd[0], cmd);
      perror("execvp failed");
      return 1;
    }
    else{
      //waitpid(pid, &child_status, 0);
      int child_exit_status;

      waitpid(pid, &child_status, WUNTRACED);
      // checks if child terminates properly,
      if (WIFEXITED(child_status)){
        child_exit_status = WEXITSTATUS(child_status);  
        /*After waitpid() returns, the macro WIFEXITED(status) can be used to check
        if the child process terminated normally (e.g., by calling exit() or returning from main()). 
        If waitpid() returns true, it means the child terminated normally (by signal, was stopped or exit() ).*/
      }
      // checks child_exit_status and do something about it
    }    
    getCPUusage();
  } 
  return 0;   
}



int shell_cd(char **args) {
  if (args[1] == NULL) {
  // No argument provided - change to home directory
    if (chdir(getenv("HOME")) != 0) {
      perror("cd");
    }
  } else {
    // Change to the specified directory
    if (chdir(args[1]) != 0) {
      perror("cd");
    }
  }
  return 1;
}

int shell_help(char **args) {
  printf("Available built-in commands:\n");
  for (int i = 0; i < num_builtin_functions(); i++) {
    printf("  %s\n", builtin_commands[i]);
  }
  return 1;
}

int shell_exit(char **args) {
  // Exit the shell
  exit(0);
}

int shell_usage(char **args) {
  if (args[1] == NULL) {
    printf("Usage: usage <command>\n");
    printf("Available commands: cd, help, exit, usage, env, setenv, unsetenv\n");
    return 1;
  }
    
  if (strcmp(args[1], "cd") == 0){
    printf("cd [directory] - Change current directory to [directory] or HOME if no argument\n");
  }else if(strcmp(args[1], "help") == 0){
    printf("help - List all available built-in commands\n");
  }else if (strcmp(args[1], "exit") == 0){
    printf("exit - Exit the shell\n");
  }else if (strcmp(args[1], "usage") == 0){
    printf("usage <command> - Show usage information for <command>\n");
  }else if (strcmp(args[1], "env") == 0){
    printf("env - List all environment variables\n");
  }else if (strcmp(args[1], "setenv") == 0){
    printf("setenv VARIABLE=value - Set environment variable VARIABLE to value\n");
  }else if (strcmp(args[1], "unsetenv") == 0){
    printf("unsetenv VARIABLE - Remove environment variable VARIABLE\n");
  }else{
    printf("Unknown command: %s\n", args[1]);
  }
  return 1;
}

int list_env(char **args) {
  for (char **env = environ; *env != NULL; env++) {
    printf("%s\n", *env);
  }
  return 1;
}

int set_env_var(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "setenv: expected argument in format VARIABLE=value\n");
    return 1;
  }

  char *equal_sign = strchr(args[1], '=');
  if (equal_sign == NULL) {
    fprintf(stderr, "setenv: format must be VARIABLE=value\n");
    return 1;
  }

  // Split variable name and value
  *equal_sign = '\0';
  char *var_name = args[1];
  char *var_value = equal_sign + 1;

  if (setenv(var_name, var_value, 1) != 0) {
    perror("setenv");
  }
    
  return 1;
}

int unset_env_var(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "unsetenv: expected variable name\n");
    return 1;
  }
    
  if (unsetenv(args[1]) != 0) {
    perror("unsetenv");
  }
    
  return 1;
}