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

    char* token = strtok(cmdline, ";");
    while (token != NULL)
    {
        cmds[numOfCmds++] = token;
        token = strtok(NULL, ";");
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
    indexInputFile = NOT_EXIST;
    indexOutputFile = NOT_EXIST;
    last = NOT_EXIST;
    customPipe = NOT_EXIST;
    redirectInput = NOT_EXIST;
    redirectOutput = NOT_EXIST;

    int i = 0;
    for (i = 0; cmd[i]; i++)
    {
        if (cmd[i] == '>')
        {
            redirectOutput = EXIST;
            if (last == 0)
                last = 1;
            if (indexOutputFile == 0)
                indexOutputFile = i; // need to know the index to get the name
        }
        if (cmd[i] == '<')
        {
            redirectInput = EXIST;
            if (indexInputFile == 0)
                indexInputFile = i; // need to know the index to get the name
        }
        if (cmd[i] == '|')
        {
            customPipe = EXIST;
        }
    }

    return customPipe;
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
    indexInputFile = NOT_EXIST;
    indexOutputFile = NOT_EXIST;
    last = NOT_EXIST;
    redirectInput = NOT_EXIST;
    redirectOutput = NOT_EXIST;
    fileOutputName = NULL;
    fileInputName = NULL;

    int tok = 0;
    int i = 0;
    for (i = 0; cmd[i]; i++) // check if there is redirection
    {
        if (cmd[i] == '<')
        {
            redirectInput = EXIST;
            if (indexOutputFile == 0)
                indexOutputFile = i;
        }
        if (cmd[i] == '>')
        {
            redirectOutput = EXIST;
            if (last == 0) // output is done last
                last = 1;
            if (indexOutputFile == 0)
                indexOutputFile = i;
        }
        if (cmd[i] == '>' && cmd[i + 1] == '>') // check for append
            last = 2;
    }

            // if there are redirections for both Input and Output
    if (redirectInput && redirectOutput)
    {
        char* token = strtok(tempCmd, " <>\t\n"); // CMD_DELIMITERS not used because need to check for all redirection

        while (token != NULL)
        {
            cmdTokens[tok++] = strdup(token);
            token = strtok(NULL, " <>\t\n");
        }

        // get filenames using the ids
        if (indexInputFile < indexOutputFile)
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

        char* token = strtok(tempCmd, "<");
        while (token != NULL)
        {
            redirectInputCmd[tok++] = token;
            token = strtok(NULL, "<");
        }

        tempCmd = strdup(redirectInputCmd[tok - 1]);
        token = strtok(tempCmd, "> |\t\n"); // the args after <
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
        char* token;

        if (last == 1) // write to file
            token = strtok(tempCmd, ">");
        else if (last == 2) // append to file
            token = strtok(tempCmd, ">>");

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

    if (redirectInput == NOT_EXIST && redirectOutput == NOT_EXIST)
        return parseCmd(strdup(cmd), cmdTokens);
    else
        return tok;
}