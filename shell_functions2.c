#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

int fork_and_exe(char **argv)
{
	pid_t child_pid;
	char *res;
	int allocated = 0;

	if (!argv || !argv[0])
		return (0);

	if (strchr(argv[0], '/'))
	{
		if (access(argv[0], X_OK) != 0)
		{
			fprintf(stderr, "%s: No such file or directory\n", argv[0]);
            return (127);
		}
		res = argv[0];
	}
	else
	{
		res = split_path(argv[0]);
		
		if (!res)
		{
			fprintf(stderr, "%s: No such file or directory\n", argv[0]);
			return (127);
		}
		allocated = 1;
	}
	
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		if (allocated)
			free(res);
		return (-1);
	}
	if (child_pid == 0)
	{
		execve(res, argv, environ);
		perror(argv[0]);
		exit(127);
	}
	else
		wait(NULL);
	if (allocated)
		free(res);
	return (0);
}