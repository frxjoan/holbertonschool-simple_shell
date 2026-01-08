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
int main(int ac, char **av)
{
	char *line = NULL;
	unsigned long line_no = 0;
	char **argv = NULL;
	size_t size = 0;
	int last_status = 0, built;
	(void)ac;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		if (getline(&line, &size, stdin) == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(last_status);
		}
		line_no++;
		argv = slicing_str(line);
		if (!argv || !argv[0])
		{
			free(argv);
			continue;
		}
		built = builtins(argv);
		if (built)
		{
			free(argv);
			free(line);
			line = NULL;
			if (built == 2)
				exit(last_status);
			continue;
		}

		last_status = run_command(argv, av[0], line_no);
		free(argv);
		continue;
	}
}
