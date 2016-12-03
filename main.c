#include "shell.h"

int main(int argc, char* argv [])
{
    while (1)
    {
        int i, j;
        char** cmds = malloc((sizeof (char)*BUFFER) * BUFFER);
        for (j = 0; j < BUFFER; j++) cmds[j] = '\0'; // NULL doesn't work
        char* cmdline = malloc((sizeof (char)*BUFFER) * BUFFER);
        printf("COP4338$ ");
        if (fgets(cmdline, BUFFER, stdin) == NULL)
        {
            perror("fgets failed");
            exit(ERROR);
        }
        int numOfCmds = parseCmdLine(cmdline, cmds);

        for (i = 0; i < numOfCmds; i++)
        {
            printf("CMD:%d\n", numOfCmds);
        }


        free(cmdline);
        free(cmds);
    }
    return NO_ERROR;
}