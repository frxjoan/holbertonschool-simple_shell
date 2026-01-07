#ifndef SHELL_H
#define SHELL_H

extern char **environ;

int count_words(char *);
char **slicing_str(char *);
int builtins(char **);
char *_getenv(const char *name);
char *split_path(char *cmd);
int fork_and_exe(char **);

#endif
