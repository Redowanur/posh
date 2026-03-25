#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

bool is_builtin(const char *command, char builtins[][10], int count) {
	for (int i = 0; i < count; i++) {
		if (strcmp(command, builtins[i]) == 0) {
			return true;
		}
	}

	return false;
}

bool find_executable_in_path(const char *command, char *full_path, size_t full_path_size) {
	char *env_path = getenv("PATH");
	if (env_path == NULL) {
		return false;
	}

	char path_copy[4096];
	snprintf(path_copy, sizeof(path_copy), "%s", env_path);

	char *saveptr;
	char *token = strtok_r(path_copy, ":", &saveptr);

	while (token != NULL) {
		snprintf(full_path, full_path_size, "%s/%s", token, command);

		if (access(full_path, X_OK) == 0) {
			return true;
		}

		token = strtok_r(NULL, ":", &saveptr);
	}

	return false;
}

int main(int argc, char *argv[]) {
	char valid_commands[10][10] = {
		"exit",
		"echo",
		"type",
	};

	int num_of_commands = sizeof(valid_commands) / sizeof(valid_commands[0]);

	while(1){
		setbuf(stdout, NULL);
	
		printf("$ ");
	
		char command[1024];
		if (fgets(command, sizeof(command), stdin) == NULL) {
			break;
		}
	
		command[strcspn(command, "\n")] = '\0';
		if (command[0] == '\0') {
			continue;
		}

		char *args[128];
		int arg_count = 0;

		char *saveptr;
		char *token = strtok_r(command, " \t", &saveptr);
		while (token != NULL && arg_count < 127) {
			args[arg_count++] = token;
			token = strtok_r(NULL, " \t", &saveptr);
		}
		args[arg_count] = NULL;

		if (arg_count == 0) {
			continue;
		}

		if (strcmp(args[0], "exit") == 0 && (args[1] == NULL || strcmp(args[1], "0") == 0)) {
			break;
		}

		if (strcmp(args[0], "echo") == 0) {
			for (int i = 1; i < arg_count; i++) {
				if (i > 1) {
					printf(" ");
				}
				printf("%s", args[i]);
			}
			printf("\n");
			continue;
		}

		if (strcmp(args[0], "type") == 0) {
			if (arg_count < 2) {
				continue;
			}

			for (int i = 1; i < arg_count; i++) {
				if (is_builtin(args[i], valid_commands, num_of_commands)) {
					printf("%s is a shell builtin\n", args[i]);
					continue;
				}

				char full_path[PATH_MAX];
				if (find_executable_in_path(args[i], full_path, sizeof(full_path))) {
					printf("%s is %s\n", args[i], full_path);
				} else {
					printf("%s: not found\n", args[i]);
				}
			}
			continue;
		}

		char full_path[PATH_MAX];
		if (find_executable_in_path(args[0], full_path, sizeof(full_path))) {
			pid_t pid = fork();
			if (pid == 0) {
				execv(full_path, args);
				exit(1);
			} else if (pid > 0) {
				int status;
				waitpid(pid, &status, 0);
			}
		} else {
	        printf("%s: command not found\n", args[0]);
		}
	}

  return 0;
}
