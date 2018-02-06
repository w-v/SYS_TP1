#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_PIPE 10
#define MAX_CHAR 200

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
  argc--;
  pid_t pid=getpid();

  if(argc) fprintf(stderr,"commande:");

  char str[MAX_CHAR];
  fgets(str, sizeof str, stdin);
  cleanUp(str);
  if(argc) fprintf(stderr,"(PID:%d) str: %s \n",pid,str);

  int pipe_fds[2];
  int k;
  int prev_pipe_read = 0;
  int npipes = 0;
  int open_pipes[MAX_PIPE];

  
  char* params[MAX_CHAR/2];
  char* cmd;
  char* next_cmd;
  char* svptr;
  char* svptr1;
  cmd = strtok_r(str,"|",&svptr);
  next_cmd=strtok_r(NULL,"|",&svptr); 

  while (cmd != NULL && npipes<MAX_PIPE ) {

    if(argc) fprintf(stderr,"\n(PID:%d) next_cmd: %s \n",pid,next_cmd);
    if(argc) fprintf(stderr,"(PID:%d) cmd: %s \n",pid,cmd);

    k = 0;
    params[0] = strtok_r(cmd," ",&svptr1);
    if(argc) fprintf(stderr,"(PID:%d) params[%d]: %s \n",pid,k,params[k]);
    while ( params[k] != NULL){ 
      k++;
      params[k] = strtok_r(NULL," ",&svptr1);
      if(argc) fprintf(stderr,"(PID:%d) params[%d]: %s \n",pid,k,params[k]);
    };
    params[k] = NULL;


    if(next_cmd != NULL){
      if (pipe(pipe_fds) == -1){
        if(argc) fprintf(stderr,"Pipe failed");
        exit(1);
      }
      open_pipes[npipes] = pipe_fds[0];
      if(argc) fprintf(stderr,"(PID:%d) created pipe wr:%d rd:%d \n",pid,pipe_fds[1],pipe_fds[0]);
    }

    pid = fork();
    if (pid<0) { perror("Erreur fork"); exit(1); }

    if (pid==0) {
      pid=getpid();
      if(argc) fprintf(stderr,"(PID:%d) next_cmd: %s \n",pid,next_cmd);
      if(argc) fprintf(stderr,"(PID:%d) cmd: %s \n",pid,cmd);

      if(argc) fprintf(stderr,"(PID:%d) reading from fd: %d\n",pid,prev_pipe_read);
      if ( dup2(prev_pipe_read,0) == -1){
        if(argc) fprintf(stderr,"Duplication failed");
        exit(1);
      }
      close(prev_pipe_read);
      close(pipe_fds[0]);

      if(next_cmd != NULL){
        if(argc) fprintf(stderr,"(PID:%d) writing to fd: %d\n",pid,pipe_fds[1]);
        if ( dup2(pipe_fds[1],1) == -1){
          if(argc) fprintf(stderr,"Duplication failed");
          exit(1);
        }
      	close(pipe_fds[1]);
      }


      if(argc) fprintf(stderr,"(PID:%d) exec: %s \n",pid,params[0]);
      if (execvp(params[0], params) != 0){
        if(argc) fprintf(stderr,"commande introuvable");
        exit(1);
      }
       
    }
    else {
      //el padre
      //sleep(1);
      close(pipe_fds[1]);
      prev_pipe_read = pipe_fds[0];
      cmd=next_cmd;
      next_cmd=strtok_r(NULL,"|",&svptr); 
      npipes++;
    }
  }

  int status, dead_chld;
  int exit_status = 0;
  char* fate[2];
  fate[0] = "happily";
  fate[1] = "horribly";

  for(int i;i<npipes;i++){

      close(open_pipes[i]);
      dead_chld = wait(&status);
      exit_status|=status;
  
      if(argc) fprintf(stderr,"(PID:%d) killed zombie child: %d \n",pid,dead_chld);
      if(argc) fprintf(stderr,"(PID:%d) he died %s \n",pid,fate[(status > 0) - (status < 0)]);

  } 

  if(argc) fprintf(stderr,"(PID:%d) I am dying %s \n",pid,fate[(exit_status > 0) - (exit_status < 0)]);
  exit(exit_status);

}
