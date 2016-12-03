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
        int fd;
        if (redirectOutput)
        {
            fd = open("test", O_CREAT | O_WRONLY | O_TRUNC);
            dup2(fd, 1);
            close(fd);
        }
        execvp(cmdTokens[0], cmdTokens);
        
        // return only when exec fails 
        perror("exec failed");
        exit(-1);
    } else if (pid > 0) // parent process
    {
        waitpid(pid, NULL, 0);

    } else // error occured
    {
        perror("fork failed");
        exit(ERROR);
    }
}