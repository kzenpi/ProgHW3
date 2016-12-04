#include "shell.h"

void execute(int tokens, char** cmdTokens)
{
    if (tokens > 0)
    {
        if (isalpha(cmdTokens[0][0]))
            runCommand(cmdTokens);
    }
    free(cmdTokens); // so that it won't execute multiple times
}

void runCommand(char** cmdTokens)
{
    if (!strcmp(cmdTokens[0], "quit") || !strcmp(cmdTokens[0], "exit"))
    {
        exit(NO_ERROR);
    }
    int pid = fork();

    if (pid == 0) // child process
    {
        int fdIn = 0;
        int fdOut = 0;

        if (redirectInput)
        {
            if ((fdIn = open(fileInputName, O_RDONLY)) < 0)
            {
                perror(fileInputName);
            }
            dup2(fdIn, STDIN_FILENO);
            close(fdIn);

            if (fdIn == ERROR)
                exit(ERROR);
        }
        if (redirectOutput)
        {
            if (last == 1) // if writing a new output
            {
                fdOut = open(fileOutputName, O_CREAT | O_WRONLY | O_TRUNC, 0666);
            } else if (last == 2) // if appending to existing file
            {
                fdOut = open(fileOutputName, O_CREAT | O_WRONLY | O_APPEND, 0666);
            }
            if (fdOut < 0)
            {
                perror(fileOutputName);
            }
            dup2(fdOut, STDOUT_FILENO);
            close(fdOut);

            if (fdOut == ERROR)
                exit(ERROR);
        }

        // executing the command
        execvp(cmdTokens[0], cmdTokens);
        // return only when exec fails 
        perror("exec failed");
        exit(ERROR);
    } else if (pid > 0) // parent process
    {
        waitpid(pid, NULL, 0);

    } else // error occured
    {

        perror("fork failed");
        exit(ERROR);
    }
}

void pipeAndRedirect(char* cmd)
{

}