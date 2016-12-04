#include "shell.h"

void execute(char** cmdTokens)
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
            fdIn = open(fileInputName, O_RDONLY);
            if (fdIn < 0)
            {
                perror(fileInputName);
            }
            dup2(fdIn, STDIN_FILENO);
            close(fdIn);

            if (fdIn == -1)
                exit(ERROR);
        }
        if (redirectOutput)
        {
            if (last == 1) // if writing a new output
            {
                fdOut = open(fileOutputName, O_CREAT | O_WRONLY | O_TRUNC);
            } else if (last == 2) // if appending to existing file
            {
                fdOut = open(fileOutputName, O_CREAT | O_WRONLY | O_APPEND);
            }
            if (fdOut < 0)
            {
                perror(fileOutputName);
            }
            dup2(fdOut, STDOUT_FILENO);
            close(fdOut);

            if (fdOut == -1)
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