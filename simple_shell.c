#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

/**
 * main - entry point of the simple shell
 *
 * Description:
 * Runs an infinite loop that displays a prompt, reads user input,
 * parses the command, and executes it in a child process.
 * Handles EOF (Ctrl+D), errors, and simple command execution
 * without PATH resolution or built-in commands.
 *
 * Return: Always 0 on normal exit
 */
int main(void)
{
	char *line = NULL;
	char **argv = NULL;
	size_t size = 0;
	int ret;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		if (getline(&line, &size, stdin) == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(0);
		}
		argv = slicing_str(line);
		if (!argv || !argv[0])
		{
			free(argv);
			continue;
		}
		if (builtins(argv))
		{
			free(argv);
			continue;
		}
		ret = fork_and_exe(argv);
		if (ret == -1 || ret == 127)
		{
			free(argv);
			continue;
		}
		free(argv);
	}
}
