#include "shell.h"

int parseCmdLine(char* cmdline, char* cmds[])
{
    printf("cmd:%s\n", cmdline);

    int numOfCmds = 0;

    // if there is no argument, return 0
    if ((cmds[0] = strtok(cmdline, "\n\t ")) == NULL)
        return 0;

    while ((cmds[++numOfCmds] = strtok(NULL, "\n\t ")) != NULL)
    {
        if (numOfCmds >= MAX_ARGS)
        {
            printf("Too many arguments!\n");
            exit(1);
        }
    }

    return numOfCmds;
}