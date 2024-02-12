/*--------------------------------------------------------------------

File: dp.c
Student name: Oleksander Turchyn
Student id: 300174825

Description:  Double pipe program.  To pipe the output from the standard
          output to the standard input of two other processes.
          Usage:  dp  <cmd1> : <cmd2> : <cmd3>
          Output from process created with cmd1 is piped to
          processes created with cmd2 and cmd3

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/* prototypes */
int doublePipe(char **,char **,char **);

/*--------------------------------------------------------------------

File: dp.c

Description: Main will parse the command line arguments into three arrays
         of strings one for each command to execv().
--------------------------------------------------------------------*/

int main(int argc, char *argv[])
{

   int i,j;         /*indexes into arrays */
   char *cmd1[10];  /*array for arguments of first command */
   char *cmd2[10];  /*array for arguments of second command */
   char *cmd3[10];  /*array for arguments of third command */
   if(argc == 1)
   {
     printf("Usage: dp <cmd1 arg...> : <cmd2 arg...> : <cmd3 arg....>\n");
     exit(1);
   }

   /* get the first command */
   for(i=1,j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found first command */
      cmd1[j]=argv[i];
   }
   cmd1[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only one command found\n");
      exit(1);
   }
   else i++;

   /* get the second command */
   for(j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found second command */
      cmd2[j]=argv[i];
   }
   cmd2[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only two commands found\n");
      exit(1);
   }
   else i++;

   /* get the third command */
   for(j=0 ; i<argc ; i++,j++) cmd3[j]=argv[i];
   cmd3[j]=NULL;
   if(j==0) /* no command after last : */
   {
      printf("Bad command syntax - missing third command\n");
      exit(1);
   }

   exit(doublePipe(cmd1,cmd2,cmd3));
}

/*--------------------------------------------------------------------------
  ----------------- You have to implement this function --------------------
  --------------------------------------------------------------------------
Function: doublePipe()

Description:  Starts three processes, one for each of cmd1, cmd2, and cmd3.
          The parent process will receive the output from cmd1 and
          copy the output to the other two processes.
-------------------------------------------------------------------------*/

//Double pipe procedure: 
// - Creates three child processes 
// - The output of of cmd1 is sent to cmd2 and cmd3 as input and then waits 
//   for them to finish

int doublePipe(char **cmd1, char **cmd2, char **cmd3) {
    // Create the pipe between cmd1 and cmd2
    int pipe1[2]; 
    // Create the pipe between cmd1 and cmd3
    int pipe2[2]; 

    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    pid_t pid1, pid2, pid3;

    // Fork process for cmd1
    pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

   //-------------------------
   // Child process for cmd1
    if (pid1 == 0) { 
        // Close unused ends of the pipes
        close(pipe1[0]); 
        close(pipe2[0]); 
        close(pipe2[1]); 

        // Redirect stdout to the write end of pipe1
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);

        // Execute cmd1
        execvp(cmd1[0], cmd1);
        perror("execvp cmd1");
        exit(EXIT_FAILURE);
    }

    // Fork process for cmd2
    pid2 = fork();

    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

   //-------------------------
   // Child process for cmd2
    if (pid2 == 0) { 
        // Close unused ends of the pipes
        close(pipe1[1]); 
        close(pipe2[0]);

        // Redirect stdin to the read end of pipe1
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        // Execute cmd2
        execvp(cmd2[0], cmd2);
        perror("execvp cmd2");
        exit(EXIT_FAILURE);
    }

    // Fork process for cmd3
    pid3 = fork();

    if (pid3 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

   //-------------------------
   // Child process for cmd3
    if (pid3 == 0) { 
        // Close unused ends of the pipes
        close(pipe1[0]); 
        close(pipe1[1]); 
        close(pipe2[1]); 

        // Redirect stdin to the read end of pipe2
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        // Execute cmd3
        execvp(cmd3[0], cmd3);
        perror("execvp cmd3");
        exit(EXIT_FAILURE);
    }

    // Close unused ends of the pipes in parent process
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Wait for all child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

   //Once finished, complete the procedure
    return 0; 
}
