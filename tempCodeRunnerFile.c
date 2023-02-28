#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

#define A_SIZE 10
#define BUFFER 100
int n = 1; // count the number of lines

/*
   passes the line of input into commands of the shell
*/
void run_line(char *line)
{
   line[strlen(line) - 1] = 0;

   char **args = (char **)malloc(A_SIZE * BUFFER * sizeof(char));

   char *sep;
   char error_msg[100] = "An error has occurred\n";
   char *path = "/bin/ls";
   int len = strlen(error_msg);

   // places the built in command into the argument
   sep = strtok(line, " ");
   int i = 0;
   while (sep != NULL)
   {
      args[i] = sep;
      sep = strtok(NULL, " ");
      i++;
   }

   // removes any newline from the string passes in
   //  for (int i = 0; i < strlen(line); i++)
   //  {
   //     if (line[i] == '\n')
   //     {
   //        line[i] = '\0';

   //    }
   // }

   // making the cd command
   if (strcmp(args[0], "cd") == 0 || strcmp(args[0], "cd\n") == 0)
   {
      if (args[1] != NULL && args[2] == NULL)
      {
         chdir(args[1]);
      }
      else
      {
         write(STDERR_FILENO, error_msg, len);
      }
   }
  
   // making the ls command
   if (strcmp(args[0], "ls") == 0 || strcmp(args[0], "ls\n") == 0)
   {
      pid_t pid = fork();

      
      if(access(path,X_OK) == 0){
      if (pid < 0)
      {
         printf("fork failed");
      }
      else if (pid == 0)
      {

         execv(path, args);

         // args[0] ps.sh
         // args NULL
         // For test 6, args would be {"p1.sh", NULL}
      }
      else
      {

         wait(&pid);
      }
      }
   }

   if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "exit\n") == 0)
   {
      
      if (args[1] != NULL)
      {
         write(STDERR_FILENO, error_msg, len);
      }
      else
      {
         exit(0);
      }
   }
}
// making the error command

int main(int argc, char *argv[])
{

   bool loop = true;       // use to run the loop continuosly
   char userInput[BUFFER]; // use to input user input
   char *line = NULL;

   size_t len = 0;
   size_t size = BUFFER;
   ssize_t read;

   if (argc == 1) // check if there is only one command argument
   {

      while (loop)
      {
         prompt();                        // wish prompt
         fgets(userInput, BUFFER, stdin); // inputs the user input into the inputstream

         if (strcmp(userInput, "exit") == 0 || strcmp(userInput, "exit\n") == 0)
         {
            loop = false;
         }
         else
         {

            run_line(userInput); // if input is valid then run the command.
         }
         /*
         first get input from user
         split input into array
         get first position  = built in function
         if it is execute the command
         else do execv
         */
      }
   }

   if (argc == 2)
   {
      FILE *fp = fopen(argv[1], "r"); // reading a file place into the second argument

      if (fp == NULL)
      {
         printf("File opening not successful"); // checks if the file is null
         exit(0);
      }
      else
      {
         while ((read = getline(&line, &len, fp)) != -1) // reads the file line by line
         {
            run_line(line);
         }
      }
      free(line);
      fclose(fp); // close the second argument file,
   }
   return 0;
}
