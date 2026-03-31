#include <loop.h>
#include <read_command.h>
#include <split_command.h>
#include <execute.h>
#include <stdio.h>
#include <stdlib.h>

void loop() {
    char *command;
    char **args;
    int status = 1;
    int arg_count;

    do {
        printf("$ ");
        command = read_command();
        if (command == NULL) {
            break;
        }

        args = split_command(command, &arg_count);
        status = execute(args, &arg_count);

        free(command);
        free(args);
    } while (status);
}