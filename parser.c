#include "shell.h"

/*
 * Parse a command line
 * 
 * cmdline: the command line to be parsed, which was input by the user
 * cmds[]: individual commands are stored in array of strings
 * 
 * return: the number of commands (i.e. words separated by space in the user input)
 */
int parseCmdLine(char* cmdline, char** cmds)
{
    int numOfCmds = 0;

    char* token = strtok(cmdline, CMD_DELIMITERS);
    while (token != NULL)
    {
        printf("token:%s\n", token);
        cmds[numOfCmds++] = token;
        token = strtok(NULL, CMD_DELIMITERS);
    }

    return numOfCmds;
}

int parseCmd(char* cmd, char** cmdTokens)
{
    int tok = 0;
    char* token = strtok(cmd, CMD_DELIMITERS);
    while (token != NULL)
    {
        cmdTokens[tok++] = token;
        token = strtok(NULL, CMD_DELIMITERS);
    }
    return tok;
}

/*
 *  Check a command to see if redirection or pipe is requested
 * 
 * cmd: the command to be checked
 * 
 * return:  1 if redirection/pipe is required
 *          0 if no redirection/pipe is required
 */

int checkPipeRedirect(char* cmd)
{
    idInput = 0;
    idOutput = 0;
    last = 0;
    customPipe = 0;
    redirectInput = 0;
    redirectOutput = 0;

    int i = 0;
    for (i = 0; cmd[i]; i++)
    {
        if (cmd[i] == '>')
        {
            redirectOutput = 1;
            if (last == 0)
                last = 1;
            if (idOutput == 0)
                idOutput = i;
        }
        if (cmd[i] == '<')
        {
            redirectInput = 1;
            if (idInput == 0)
                idInput = i;
        }
        if (cmd[i] == '|')
        {
            customPipe = 1;
        }

    }

    if (redirectInput || redirectOutput)
        return 1;
    else
        return 0;

}

/*
 * Parse a command for redirection.  The command is tokenized
 * 
 * cmd: the command to be parsed
 * cmdTokens:   where the tokens will be stored
 * 
 * return:  the number of tokens
 */
int parseForRedirect(char* cmd, char** cmdTokens)
{
    char* tempCmd = strdup(cmd);
    idInput = 0;
    idOutput = 0;
    last = 0;
    customPipe = 0;
    redirectInput = 0;
    redirectOutput = 0;
    fileOutputName = "\0";
    fileInputName = "\0";

    int tok = 0;

    int i = 0;
    for (i = 0; cmd[i]; i++) // check if there is redirection
    {
        if (cmd[i] == '<')
        {
            redirectInput = 1;
            if (idOutput == 0)
                idOutput = i;
        }
        if (cmd[i] == '>')
        {
            redirectOutput = 1;
            if (last == 0) // output is done last
                last = 1;
            if (idOutput == 0)
                idOutput = i;
        }
        if (cmd[i] == '>' && cmd[i + 1] == '>') // check for append
            last = 2;
    }

    // if there are redirections for both Input and Output
    if (redirectOutput && redirectOutput)
    {
        char* token = strtok(tempCmd, " <>\t\n"); // CMD_DELIMITERS not used because need to check for all redirection

        while (token != NULL)
        {
            cmdTokens[tok++] = strdup(token);
            token = strtok(tempCmd, " <>\t\n");
        }

        // get filenames using the ids
        if (idInput < idOutput)
        {
            fileInputName = strdup(cmdTokens[tok - 2]);
            fileOutputName = strdup(cmdTokens[tok - 1]);
        } else
        {
            fileInputName = strdup(cmdTokens[tok - 1]);
            fileOutputName = strdup(cmdTokens[tok - 2]);
        }

        // take out the file names from the command line
        cmdTokens[tok - 2] = NULL;
        cmdTokens[tok - 1] = NULL;
        return tok - 2;
    }

    // standardinput
    if (redirectInput)
    {
        char** redirectInputCmd = malloc((sizeof (char)*BUFFER) * BUFFER);
        char* tempCmd = strdup(cmd);

        char* token = strtok(cmd, "<");
        while (token != NULL)
        {
            redirectInputCmd[tok++] = token;
            token = strtok(NULL, "<");
        }

        tempCmd = strdup(redirectInputCmd[tok - 1]);
        token = strtok(cmd, "> |\t\n"); // the args after <
        fileInputName = strdup(token); // FILE NAME IS AFTER >

        tok = 0;
        token = strtok(redirectInputCmd[0], CMD_DELIMITERS);
        while (token != NULL)
        {
            cmdTokens[tok++] = strdup(token);
            token = strtok(NULL, CMD_DELIMITERS);
        }

        free(redirectInputCmd);
    }

    // standardoutput and append
    if (redirectOutput)
    {
        char** redirectOutputCmd = malloc((sizeof (char)*BUFFER) * BUFFER);
        char* tempCmd = strdup(cmd);

        char* token = strtok(tempCmd, ">");
        while (token != NULL)
        {
            redirectOutputCmd[tok++] = token;
            if (last == 1)
                token = strtok(NULL, ">");
            else if (last == 2)
                token = strtok(NULL, ">");
        }

        tempCmd = strdup(redirectOutputCmd[tok - 1]);
        token = strtok(tempCmd, "< |\t\n"); // the args after >
        fileOutputName = strdup(token); // FILE NAME IS AFTER >

        // put them in cmdTokens
        tok = 0;
        token = strtok(redirectOutputCmd[0], CMD_DELIMITERS);
        while (token != NULL)
        {
            cmdTokens[tok++] = token;
            token = strtok(NULL, CMD_DELIMITERS);
        }

        free(redirectOutputCmd);
    }

    if (!redirectInput && !redirectOutput)
        return parseCmd(strdup(cmd), cmdTokens);
    else
        return tok;
}