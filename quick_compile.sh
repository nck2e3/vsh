#!/bin/bash

# Define output executable
OUTPUT="my_shell"

# Compile the C files
gcc -o $OUTPUT shell.c custom_commands.c

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Output file: $OUTPUT"
else
    echo "Compilation failed."
fi
