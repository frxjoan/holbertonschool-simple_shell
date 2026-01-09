#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

/**
 * main - Entry point of the simple shell program
 * @ac: Argument count
 * @av: Argument vector
 *
 * Description: This function runs an infinite loop that displays
 * a prompt, reads user input, parses commands, handles built-in
 * commands, and executes external programs.
 *
 * Return: Always returns 0 on success, or exits with the last
 * command status on failure
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
