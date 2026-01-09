#include <stdio.h>
#include "shell.h"

/**
 * _printenv - Print the environment variables
 * @environ: Array of environment variables
 *
 * Description: This function prints each environment variable on
 * a separate line to the standard output.
 *
 * Return: Nothing
 */
void _printenv(char **environ)
{
	unsigned int i = 0;

	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}
