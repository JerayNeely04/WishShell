#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

char *paths[BUFFER] = {"/bin", NULL};
/*
prints out the wish prompt
*/
void prompt(){

   printf("wish> ");
}

/*
searches the path and breaks the path into arguements
*/
char *searchPath(char path[], char *command)
{
   char *temp = (char *)malloc(100 * sizeof(char));
   char *sep;
   char **args = (char **)malloc(A_SIZE * BUFFER * sizeof(char));

   if (access(command, X_OK) == 0)
   {
      return command;
   }
   else if (access(command, X_OK) != 0)
   {

      int i = 0;
      sep = strtok(*paths, " ");
      while (sep != NULL)
      {
         args[i] = sep;
         strcpy(temp, sep);
         strcat(temp, "/");
         strcat(temp, command);
         sep = strtok(NULL, " ");

         if (access(temp, X_OK) == 0)
         {
            return temp;
         }
      }
   }
   return "-1";
}
