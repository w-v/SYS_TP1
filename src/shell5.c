#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_PIPE 10
#define MAX_CHAR 200

// function for removing the trailing newline in fgets' result
int cleanUp(char* str){
  int i = 0;
  while (str[i] != '\n'){
    i++;
  }
  str[i] = '\0';
  return 0;
}


int main (int argc, char* argv[])
{
  /* to obtain a boolean indicating wether there is an argument.
   * Used to enable verbose mode
   */
  argc--;
  
  pid_t pid=getpid();
  
  if(argc) fprintf(stderr,"commande:");

  char str[MAX_CHAR];
  fgets(str, sizeof str, stdin);
  cleanUp(str);
  if(argc) fprintf(stderr,"(PID:%d) str: %s \n",pid,str);

  int pipe_fds[2];
  
  // index used to parse commands' parameters
  int k;

  // to keep track of the reading end of the last pipe
  int prev_pipe_read = 0;

  // pipe counter
  int npipes = 0;

  // array of reading file descriptors for each pipe in the parent process
  int open_pipes[MAX_PIPE];

  // array for command parameters
  char* params[MAX_CHAR/2];

  char* cmd;

  // needed to check wether the output of a command is to be piped
  // to the next one or printed
  char* next_cmd;

  // pointer keeping the context for strtok_r()
  char* svptr;
  char* svptr1;

  cmd = strtok_r(str,"|",&svptr);
  next_cmd = strtok_r(NULL,"|",&svptr); 

  // this loop is treating each command 
  while (cmd != NULL && npipes<MAX_PIPE ) {
    
    if(argc) fprintf(stderr,"\n(PID:%d) next_cmd: %s \n",pid,next_cmd);
    if(argc) fprintf(stderr,"(PID:%d) cmd: %s \n",pid,cmd);

    // parameter parsing
    k = 0;
    params[0] = strtok_r(cmd," ",&svptr1);
    
    if(argc) fprintf(stderr,"(PID:%d) params[%d]: %s \n",pid,k,params[k]);
    while ( params[k] != NULL){ 
      k++;
      params[k] = strtok_r(NULL," ",&svptr1);
      if(argc) fprintf(stderr,"(PID:%d) params[%d]: %s \n",pid,k,params[k]);
    };
    params[k] = NULL;

    // pipe creation
    if(next_cmd != NULL){
      if (pipe(pipe_fds) == -1){
        if(argc) fprintf(stderr,"Pipe failed");
        exit(1);
      }
      open_pipes[npipes] = pipe_fds[0];
      if(argc) fprintf(stderr,"(PID:%d) created pipe wr:%d rd:%d \n",pid,pipe_fds[1],pipe_fds[0]);
    }

    // making a child process to execute the current command
    pid = fork();
    if (pid < 0) { perror("Erreur fork"); exit(1); }

    if (pid == 0) {
      // child process
      pid = getpid();

      // stderr is used to print debug informations to the screen rather than the pipe and not alter the outpout piped to the next process
      if(argc) fprintf(stderr, "(PID:%d) next_cmd: %s \n", pid, next_cmd);
      if(argc) fprintf(stderr, "(PID:%d) cmd: %s \n", pid, cmd);

      // reading file descriptor duplication
      // connecting input to previous pipe
      if(argc) fprintf(stderr, "(PID:%d) reading from fd: %d\n", pid, prev_pipe_read);
      if(dup2(prev_pipe_read,0) == -1){
        if(argc) fprintf(stderr, "Duplication failed");
        exit(1);
      }
      // closing the last duplicated file descriptor
      close(prev_pipe_read);
      // closing the reading file descriptor of the next pipe
      // this process doesn't read from it
      close(pipe_fds[0]);

      // writing file descriptor duplication
      // connecting output to next pipe
      if(next_cmd != NULL){
        if(argc) fprintf(stderr, "(PID:%d) writing to fd: %d\n", pid, pipe_fds[1]);
        if (dup2(pipe_fds[1], 1) == -1){
          if(argc) fprintf(stderr, "Duplication failed");
          exit(1);
        }
      	close(pipe_fds[1]);
      }

      // command execution
      if(argc) fprintf(stderr,"(PID:%d) exec: %s \n",pid,params[0]);
      if (execvp(params[0], params) != 0){
        if(argc) fprintf(stderr,"commande introuvable");
        exit(1);
      }

    }

    else {
      // parent process

      pid = getpid();
      // closing the writing file descriptor
      close(pipe_fds[1]);
      // reading file descriptor is kept for the next child process
      prev_pipe_read = pipe_fds[0];
      // parsing the next command
      cmd = next_cmd;
      next_cmd = strtok_r(NULL,"|",&svptr); 
      npipes++;
    }
  }

  int status, dead_chld;
  int exit_status = 0;
  char* fate[2];
  fate[0] = "happily";
  fate[1] = "horribly";

  for(int i;i<npipes;i++){

      close(open_pipes[i]);             // closing the remaining file descriptors
      dead_chld = wait(&status);        // waiting for zombies 
      exit_status |= status;            // computing the program exit status
  
      if(argc) fprintf(stderr,"(PID:%d) killed zombie child: %d \n",pid,dead_chld);
      if(argc) fprintf(stderr,"(PID:%d) he died %s (code: %d)\n",pid,fate[(status > 0) - (status < 0)],status);

  } 
  
  // signum function :
  // x  -> (x > 0) - (x < 0) 
  // 0  -> 0
  // >0 -> 1
  if(argc) fprintf(stderr,"(PID:%d) I am dying %s \n",pid,fate[(exit_status > 0) - (exit_status < 0)]);
  exit(exit_status);

}
