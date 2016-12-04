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
        int numOfCmds = parseCmdLine(cmdline, cmds); // parse commandline, into commands

        for (i = 0; i < numOfCmds; i++)
        {
            fileInputName = NULL;
            fileOutputName = NULL;
            numOfPipes = 0;

            char* tempCmd = strdup(cmds[i]);
            char** cmdTokens = malloc((sizeof (char)*BUFFER) * BUFFER);
            for (j = 0; j < BUFFER; j++)
                cmdTokens[j] = '\0';

            if (checkPipeRedirect(strdup(cmds[i])) == EXIST) // PIPE
            {
                printf("YES PIPE\n");

            } else // NO PIPE
            {
                if (redirectInput == EXIST || redirectOutput == EXIST) // REDIRECTION
                {
                    int tokens = parseForRedirect(strdup(tempCmd), cmdTokens);
                    printf("filename: %s\n", fileInputName);
                    printf("before execute\n");
                    execute(tokens, cmdTokens);
                    printf("after execute\n");
                    printf("filename: %s\n", fileInputName);

                } else // NO REDIRECTION
                {
                    int tokens = parseCmd(strdup(cmds[i]), cmdTokens);
                    execute(tokens, cmdTokens);
                }
            }
        }

        free(cmdline);
        free(cmds);
    }
    return NO_ERROR;
}