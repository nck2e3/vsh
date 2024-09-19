#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

//Function pointer typedef for custom commands...
typedef void (*command_func)();

//Struct to define custom commands and their handlers...
struct command {
    char* name;
    command_func handler;
};

//Function prototypes for custom commands...
void mycmd_handler();
void hello_handler();
void help_handler();
void date_handler();
void rainbow_handler();
void cd_handler();

//Function to execute custom commands...
int execute_custom_command(char* input);

//External declaration of custom command table...
extern struct command custom_commands[];

#endif //CUSTOM_COMMANDS_H
