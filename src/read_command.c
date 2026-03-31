#include <stdio.h>
#include <read_command.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

char *read_command(){
    int size = BUFFER_SIZE;
    int i=0;
    char *command = malloc(sizeof(char) * size);

    if(!command){
        fprintf(stderr, "posh: allocation error\n");
        exit(1);
    }

    while(1){
        int ch = getchar();

        if(ch == EOF || ch == '\n'){
            if (ch == EOF && i == 0) {
                free(command);
                return NULL;
            }
            command[i] = '\0';
            return command;
        }
        else{
            command[i] = ch;
        }
        i++;

        if(i >= size){
            size+=BUFFER_SIZE;
            command = realloc(command, size);
            if(!command){
                fprintf(stderr, "posh: allocation error\n");
                exit(1);
            }
        }
    }
}