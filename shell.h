#ifndef SHELL_H
#define SHELL_H

extern char **environ;

int count_words(char *);
char **slicing_str(char *);
int builtins(char **);
char *_getenv(const char *);
char *split_path(const char *);
int run_command(char **, const char *, unsigned long);
int precheck_path(const char *, int *);
int cleanup_and_return(int , char *, int);
int resolve_cmd(const char *, char **, int *);

#endif
