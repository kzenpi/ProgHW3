#include "shell.h"

int main(int argc, char* argv [])
{
    char cmdline[BUFSIZ];
    char* cmds[MAX_ARGS];

    for (;;)
    {
        printf("COP4338$ ");
        if (fgets(cmdline, BUFSIZ, stdin) == NULL)
        {
            perror("fgets failed");
            exit(1);
        }

        //        int numOfCmds = parseCmdLine(cmdline, cmds);
        //        printf("cmd:%s\n", cmdline);
        //        printf("num:%d\n", numOfCmds);
        execute(cmdline);
    }
    return 0;
}