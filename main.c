#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

int n = 1; // count the number of lines

/*
updates the path of the code

*/
void updatepath(char *token[])
{
   int i = 0;

   while (1)
   {

      if (token[i] == NULL)
      {
         paths[i] = NULL;
         break;
      }
      if (token[i] != NULL)
      {

         paths[i] = (char *)malloc(strlen(token[i]) + 1);
         strcpy(paths[i], token[i]);
      }
      i++;
   }
}

/*
   passes the line of input into commands of the shell
*/
void run_line(char *line)
{

   line[strlen(line) - 1] = 0;// terminates the new line character
   char **args = (char **)malloc(A_SIZE * BUFFER * sizeof(char)); //string array that hold the argument commands

   char *sep;
   char error_msg[100] = "An error has occurred\n";
   char *path = "/bin";
   int len = strlen(error_msg);

   // places the built in command into the argument array
   sep = strtok(line, " ");
   int i = 0;
   while (sep != NULL)
   {

      args[i] = sep;
      sep = strtok(NULL, " ");
      i++;
   }
   
   // path built in command
   //displays the path when user type path
   if (strcmp(args[0], "path") == 0)
   {
      updatepath(args + 1);
   }
   /*
   makes the cd command if arguemnt 1 isnt null and arguemnt is null
   change directory to the next directory
   */
   else if (strcmp(args[0], "cd") == 0)
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
   else if (strcmp(args[0], "exit") == 0)
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
   else
   {
      char *temp = searchPath(path, args[0]); //temps holds the first command of the path
      /*
      if temp can be access then make a new
      */
      if (strcmp(temp, "-1") == 0)
      {
         write(STDERR_FILENO, error_msg, len);
      }

      if (access(temp, X_OK) == 0)
      {
         int rc = fork();
         if (rc < 0)
         {

            printf("fork failed");
         }

         if (rc == 0)
         {
            execv(temp, args);
         }
         else
         {

            wait(NULL);
         }
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

   if (argc == 1)
   { // check if there is only one command argument

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

         while ((read = getline(&line, &len, fp)) != -1)
         { // reads the file line by line

            run_line(line);
         }
      }
      free(line);
      fclose(fp); // close the second argument file,
   }
   return 0;
}
