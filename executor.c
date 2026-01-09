#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

/**
 * resolve_cmd - resolves a command to an executable path
 * @cmd: command name entered by the user
 * @out_path: address where the resolved path will be stored
 * @allocated: flag set to 1 if out_path was dynamically allocated
 *
 * Description:
 * If the command contains a '/', it is treated as a direct path.
 * Otherwise, the PATH environment variable is searched to find
 * an executable matching the command name.
 *
 * Return:
 * 0 on success,
 * 127 if the command is not found,
 * 1 on memory allocation failure,
 * -1 on invalid arguments.
 */
int resolve_cmd(const char *cmd, char **out_path, int *allocated)
{
	if (out_path == NULL || !cmd)
		return (-1);

	if (strchr(cmd, '/'))
	{
		*out_path = strdup(cmd);
		if (!*out_path)
			return (1);
		*allocated = 1;
		return (0);
	}
	*out_path = split_path(cmd);
	if (!*out_path)
		return (127);
	*allocated = 1;
	return (0);
}

/**
 * validate_exec_path - checks validity of an executable path
 * @path: resolved executable path
 * @is_dir: flag set to 1 if path refers to a directory
 *
 * Description:
 * Verifies that the file exists, is not a directory,
 * and has execute permissions before execution.
 *
 * Return:
 * 0 if the path is valid and executable,
 * 126 if permission is denied or path is a directory,
 * 127 if the file does not exist.
 */
int validate_exec_path(const char *path, int *is_dir)
{
	struct stat st;

	*is_dir = 0;
	if (stat(path, &st) != 0)
		return (127);
	if (S_ISDIR(st.st_mode))
	{
		*is_dir = 1;
		return (126);
	}
	if (access(path, X_OK) != 0)
		return (126);
	return (0);
}

/**
 * cleanup_and_return - frees allocated path if needed and returns a code
 * @allocated: flag indicating if path was dynamically allocated
 * @path: executable path to free if allocated
 * @code: status code to return
 *
 * Description:
 * Utility function used to avoid code duplication by
 * freeing memory when required before returning an error code.
 *
 * Return: the provided code.
 */
int cleanup_and_return(int allocated, char *path, int code)
{
	if (allocated)
		free(path);
	return (code);
}

/**
 * print_err - prints a formatted error message to stderr
 * @shell: name of the shell executable
 * @line_no: current command line number
 * @cmd: command that caused the error
 * @is_dir: boolean to identificate directory
 * @code: status code associated with the error
 *
 * Description:
 * Prints errors using the required shell format:
 * shell: line: command: message
 *
 * Return: the provided status code.
 */
int print_err(const char *shell, unsigned long line_no,
const char *cmd, int code, int is_dir)
{
	if (code == 127)
		fprintf(stderr, "%s: %lu: %s: not found\n", shell, line_no, cmd);
	else if (is_dir)
		fprintf(stderr, "%s: %lu: %s: Is a directory\n", shell, line_no, cmd);
	else
		fprintf(stderr, "%s: %lu: %s: Permission denied\n", shell, line_no, cmd);
	return (code);
}

/**
 * run_command - resolves, validates and executes a command
 * @argv: NULL-terminated argument vector
 * @shell: name of the shell executable
 * @line_no: current command line number
 *
 * Description:
 * This function resolves the command path, performs pre-execution
 * checks (existence, permissions, directory), forks a child process,
 * executes the command, and retrieves the exit status.
 *
 * It ensures that fork is not called if the command is invalid.
 *
 * Return:
 * Exit status of the executed command,
 * 126 or 127 on execution errors,
 * 0 if no command is provided.
 */
int run_command(char **argv, const char *shell, unsigned long line_no)
{
	pid_t pid;
	char *path;
	int rc, status, allocated = 0, is_dir = 0;

	if (!argv || !argv[0])
		return (0);
	rc = resolve_cmd(argv[0], &path, &allocated);
	if (rc == 127)
		return (print_err(shell, line_no, argv[0], 127, is_dir));
	if (rc)
		return (rc);
	rc = validate_exec_path(path, &is_dir);
	if (rc)
		return (cleanup_and_return(allocated, path,
	print_err(shell, line_no, argv[0], rc, is_dir)));
	pid = fork();
	if (pid < 0)
		return (cleanup_and_return(allocated, path, 1));
	if (pid == 0)
	{
		execve(path, argv, environ);
		if (errno == EACCES || errno == EISDIR)
			_exit(126);
		_exit(127);
	}
	if (waitpid(pid, &status, 0) < 0)
		return (cleanup_and_return(allocated, path, 1));
	if (allocated)
		free(path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
