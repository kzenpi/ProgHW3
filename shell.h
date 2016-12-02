#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 20
#define BUFSIZ 1024

int parseCmdLine(char* cmdline, char* cmds[]);
void execute(char* cmdline);

#endif /* SHELL_H */

