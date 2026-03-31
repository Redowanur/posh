#include <split_command.h>
#include <stdlib.h>
#include <string.h>

char **split_command(char *command, int *arg_count){
    char **args = malloc(sizeof(char*) * 128);

    *arg_count = 0;

    if (command == NULL) {
        args[0] = NULL;
        return args;
    }

    char *saveptr;
    char *token = strtok_r(command, " \t", &saveptr);

    while (token != NULL && *arg_count < 127) {
        args[(*arg_count)++] = token;
        token = strtok_r(NULL, " \t", &saveptr);
    }

    args[*arg_count] = NULL;

    return args;
}