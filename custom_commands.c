#include <stdio.h>
#include <string.h>
#include "custom_commands.h"

// Function for 'mycmd'...
void mycmd_handler() {
    printf("Executing my custom command!\n");
}

// Function for 'hello'...
void hello_handler() {
    printf("Hello, world!\n");
}

// Custom command table definition...
struct command custom_commands[] = {
    {"mycmd", mycmd_handler},
    {"hello", hello_handler},
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
