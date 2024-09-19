//shell.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "custom_commands.h"  //Include the custom commands header

#define RESET_COLOR "\033[0m"          //White for RESET...
#define USER_COLOR "\033[1;32m"        //Bright green for username...
#define FORMATTING_COLOR "\033[0m"     //White for the formatting...
#define HOST_COLOR "\033[1;34m"        //Bright blue for the hostname...
#define ERROR_COLOR "\033[1;31m"       //Bright red for errors...

// Function prototypes...
void child_repl();  
void get_user_and_hostname(char*, size_t);

int main(void) {
    pid_t pid;
    int status;

    //Before forking...
    printf("Before fork: I am process with PID %d, PPID %d\n", getpid(), getppid());
    pid = fork();

    switch (pid) {
        case -1:
            //Fork failed...
            perror("Fork failed");
            return -1;

        case 0:
            //Child process...
            child_repl(); //Repl loop...
            printf("Child process: Exiting now.\n");
            return 0;

        default:
            //Parent process...
            printf("Parent process: I am the parent with PID %d, my child is PID %d\n", getpid(), pid);
            printf("Parent process: Waiting for the child to finish...\n");
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid() failed");
                return 1;
            }
            if (WIFEXITED(status)) {
                printf("Parent process: Child exited with status: %d\n", WEXITSTATUS(status));
            }
            printf("Parent process: Finished waiting for the child.\n");
    }

    //After fork...
    printf("After fork: I am process with PID %d, PPID %d\n", getpid(), getppid());

    return 0;
}

// Function to get the username and hostname, then format the prompt...
void get_user_and_hostname(char* prompt, size_t prompt_len) {
    char hostname[HOST_NAME_MAX];
    char* user;

    //Get the hostname...
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("gethostname(...) failure!");
        exit(1);
    }

    //Get the current user...
    user = getenv("USER");
    if (user == NULL) {
        perror("getenv(USER) failure!");
        exit(1);
    }

    //Construct the prompt with colored username and hostname...
    snprintf(prompt, prompt_len, "%s%s%s@%s%s%s", 
             USER_COLOR, 
             user, 
             FORMATTING_COLOR, 
             HOST_COLOR, 
             hostname, 
             RESET_COLOR);
}

//REPL loop function for handling shell input and execution...
void child_repl() {
    char input[256];
    char* args[10];
    pid_t pid;
    int status;
    char prompt[512];  //Assuming the total prompt won't exceed 511 characters...

    //Get the username and hostname with color...
    get_user_and_hostname(prompt, sizeof(prompt));
       
    while (1) {
        //Prompt for user input...
        printf("%s%s ▷ %s", prompt, FORMATTING_COLOR, RESET_COLOR);
        fflush(stdout);

        //Get user input...
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets(...) failure!");
            exit(1);
        }

        //Remove the newline character...
        input[strcspn(input, "\n")] = '\0';

        //If the user types "exit", break out of the loop...
        if (strcmp(input, "exit") == 0) {
            printf("Exiting shell.\n");
            break;
        }
        //Check if the command is `cd`...
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: expected argument\n");
            } else {
                cd_handler(args[1]);
            }
            continue;
        }        

        //Check if the input is a custom command...
        if (execute_custom_command(input)) { 
            //execute_custom_command is from "custom_commands.h"
            continue;  //Skip the execvp() part for custom commands...
        }        

        //Tokenize input into command and arguments...
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        //Fork a new process to run the command...
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "%sERROR: fork(...) failure! ", ERROR_COLOR);  // Use fprintf for formatted output
            perror("");  // perror automatically prints the system error message
            exit(1);
        } else if (pid == 0) {
            //Child process: execute the command...
            if (execvp(args[0], args)) {
                fprintf(stderr, "%sERROR: execvp(...) failure! ", ERROR_COLOR);  // Use fprintf for formatted output
                perror("");  // perror prints the error message
                exit(1);
            }
        } else {
            //Parent process: wait for the child to finish...
            if (waitpid(pid, &status, 0) == -1) {
                fprintf(stderr, "%sERROR: waitpid(...) failure! ", ERROR_COLOR);  // Use fprintf for formatted output
                perror("");  // perror prints the error message
            }
        } 
    }
}
