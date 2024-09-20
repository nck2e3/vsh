#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include "custom_commands.h"

#define RESET_COLOR "\033[0m"
#define RAINBOW_COLORS 7 

// Function for 'mycmd'...
void mycmd_handler() {
    printf("Executing my custom command!\n");
}

void cd_handler(char* path) {
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

// Function for 'hello'...
void hello_handler() {
    printf("Hello, world!\n");
}

// Function for 'help'...
void help_handler() {
    printf("Available commands:\n");
    printf(" - help: Displays this help message\n");
    printf(" - exit: Exits the shell\n");
    printf(" - hello: Prints 'Hello, world!'\n");
    printf(" - date: Gives the date\n");
    printf(" - rainbow: Displays rainbow text\n");
    printf(" - stand: A question...\n");
    printf(" - Any system command (e.g., ls, pwd, cat, etc.)\n");
}

// Function for 'date'...
void date_handler() {
    system("date");
}

// Function for 'rainbow'...
void rainbow_handler() {
    static const char* colors[RAINBOW_COLORS] = {
        "\033[1;31m", // Red
        "\033[1;33m", // Yellow
        "\033[1;32m", // Green
        "\033[1;36m", // Cyan
        "\033[1;34m", // Blue
        "\033[1;35m", // Magenta
        "\033[1;38;5;208m"  // Orange
    };

    const char* message = "Rainbow mode activated!";
    int message_len = strlen(message);

    for (int i = 0; i < message_len; i++) {
        printf("%s%c", colors[i % RAINBOW_COLORS], message[i]);
        fflush(stdout);
        usleep(100000); // Sleep for 100 milliseconds
    }
    printf("%s\n", RESET_COLOR); // Reset color and move to the next line
}

void stand_handler() {
    char x[100]; //Allocate memory for input string (really a char array)...
    printf("What do you stand on?\n");
    fgets(x, sizeof(x), stdin); //Get user input from input stream...
    printf("Prove that you stand on %s", x);
}

// Custom command table definition...
struct command custom_commands[] = {
    {"mycmd", mycmd_handler},
    {"hello", hello_handler},
    {"help", help_handler},
    {"date", date_handler},
    {"rainbow", rainbow_handler}, // Add the rainbow command...
    {"cd", cd_handler}, 
     {"stand", stand_handler}, 
    {NULL, NULL} // End of the table...
};

// Function to execute custom commands...
int execute_custom_command(char* input) {
    for (int i = 0; custom_commands[i].name != NULL; i++) {
        if (strcmp(input, custom_commands[i].name) == 0) {
            custom_commands[i].handler();  // Execute the handler function...
            return 1;  // Indicate that a custom command was found and executed...
        }
    }
    return 0;  // No custom command found...
}
