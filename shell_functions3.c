#include <stdio.h>
#include "shell.h"

void _printenv(char **environ)
{
	unsigned int i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}
