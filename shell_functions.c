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
