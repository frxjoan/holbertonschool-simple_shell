#include "shell.h"
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

int builtins(char **argv)
{
	if (strcmp(argv[0], "exit") == 0 && !argv[1])
		return (2);
	return (0);

	if (strcmp(argv[0], "env") == 0 && !argv[1])
		_printenv(environ);
	return (0);
}

char *_getenv(const char *name)
{
	unsigned int i = 0;
	char *ptr = environ[0];
	int len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0)
		{
			if (environ[i][len] == '=')
			{
				ptr = environ[i] + len + 1;
				return (ptr);
			}
		}
		i++;
	}
	return (NULL);
}

char *split_path(const char *cmd)
{
	char *path = _getenv("PATH");
	char *path_cpy, *dir, *res;

	if (!path || !cmd || cmd[0] == '\0')
		return (NULL);

	path_cpy = strdup(path);
	if (!path_cpy)
		return (NULL);

	dir = strtok(path_cpy, ":");
	while (dir)
	{
		res = malloc(strlen(dir) + strlen(cmd) + 2);
		if (!res)
		{
			free(path_cpy);
			return (NULL);
		}
		sprintf(res, "%s/%s", dir, cmd);

		if (access(res, X_OK) == 0)
		{
			free(path_cpy);
			return (res);
		}
		free(res);
		dir = strtok(NULL, ":");
	}
	free(path_cpy);
	return (NULL);
}
