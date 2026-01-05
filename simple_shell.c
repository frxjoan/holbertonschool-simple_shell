#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

/**
 * count_words - counts the number of words in a string
 * @str: input string
 *
 * Description:
 * Counts how many space- or newline-separated words are present
 * in the given string.
 *
 * Return: number of words found
 */
int count_words(char *str)
{
	int count = 0;
	int in_word = 0;

	while (*str)
	{
		if (*str != ' ' && *str != '\n' && *str != '\t' && !in_word)
		{
			count++;
			in_word = 1;
		}
		else if (*str == ' ' || *str == '\n' || *str == '\t')
			in_word = 0;
		str++;
	}
	return (count);
}

/**
 * slicing_str - splits a string into tokens
 * @str: input string to split
 *
 * Description:
 * Splits the given string into an array of words separated by
 * spaces, newlines or tabs. The original string is modified.
 *
 * Return: NULL-terminated array of tokens, or NULL on failure
 */
char **slicing_str(char *str)
{
	int i = 0;
	char **tokens;
	char *token;
	int words;

	words = count_words(str);
	tokens = malloc(sizeof(char *) * (words + 1));

	if (tokens == NULL)
		return (NULL);

	token = strtok(str, " \n\t");
	while (token)
	{
		tokens[i++] = token;
		token = strtok(NULL, " \n\t");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * prompt - displays the shell prompt
 *
 * Description:
 * Prints the current user name and working directory followed by
 * a dollar sign. If the working directory cannot be retrieved,
 * a '?' is displayed instead.
 */
void prompt(void)
{
	struct passwd *pw = getpwuid(getuid());
	char cwd[1024];
	char *user;

	if (!pw)
		user = "unknown";
	else
		user = pw->pw_name;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		printf("%s:?$ ", user);
	else
		printf("%s:%s$ ", user, cwd);
	fflush(stdout);
}

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
	pid_t child_pid;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			prompt();
		if (getline(&line, &size, stdin) == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(0);
		}
		argv = slicing_str(line);
		if (!argv || !argv[0] || argv[1])
		{
			free(argv);
			continue;
		}
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error:");
			free(argv);
			continue;
		}
		if (child_pid == 0)
		{
			execve(argv[0], argv, NULL);
			perror("execve");
			exit(1);
		}
		else
			wait(NULL);
		free(argv);
	}
}
