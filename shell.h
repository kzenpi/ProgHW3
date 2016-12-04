#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>


/*********************************************/
#define MAX_ARGS 20
#define BUFFER 1024

#define NO_ERROR 0
#define ERROR -1

#define EXIST 1
#define NOT_EXIST 0

#define CMD_DELIMITERS " \t\n"


/**********     Variables   *****/
int numOfPipes;
int redirectInput;
int redirectOutput;
int idInput; // know where the < is in comdline
int idOutput; // know where the > is in cmdline
int last; //   to make sure output is done last
int customPipe; // pipe is already in unistd.h
char* fileInputName;
char* fileOutputName;


/*********  FUNCTIONS   ***************/
// parser.c
int parseCmdLine(char* cmdline, char** cmds);
int parseCmd(char* cmd, char** cmdTokens);
int checkPipeRedirect(char* cmd);
int parseForRedirect(char* cmd, char** cmdTokens);

// execute.c
void execute(char** cmdTokens);
#endif /* SHELL_H */

