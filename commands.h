#ifndef _PROMPT_H
#define _PROMPT_H

#define A_SIZE 10
#define BUFFER 100

extern char *paths[BUFFER];
/*
prints out the command prompt
*/
void prompt();
/*
Searches for th epath and spits the path by directories
*/
char *searchPath(char path[], char *command);

/*
updates the path directory 
*/
void updatepath(char *token[]);
#endif