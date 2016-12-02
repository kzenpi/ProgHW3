#include "shell.h"

void execute(char* cmdline)
{
    int pid, async;
    char* args[MAX_ARGS];


    int nargs = parseCmdLine(cmdline, args);
    if (nargs <= 0) return;

    if (!strcmp(args[0], "quit") || !strcmp(args[0], "exit"))
    {
        exit(0);
    }

    /* check if async call */
    if (!strcmp(args[nargs - 1], "&"))
    {
        async = 1;
        args[--nargs] = 0;
    } else async = 0;

    pid = fork();
    if (pid == 0)
    { /* child process */
        execvp(args[0], args);
        /* return only when exec fails */
        perror("exec failed");
        exit(-1);
    } else if (pid > 0)
    { /* parent process */
        if (!async) waitpid(pid, NULL, 0);
        else printf("this is an async call\n");
    } else
    { /* error occurred */
        perror("fork failed");
        exit(1);
    }
}