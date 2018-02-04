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

  int arrayForPipe[2];
  int k;
  int lectureDuPipePred = 0;
  int nbPipe = 0;

  char* params[MAX_CHAR/2];
  char* cmd = (char *) malloc(sizeof(char)*MAX_CHAR);
  char* next_cmd;
  char* svptr;
  char* svptr1;
  cmd = strtok_r(str,"|",&svptr);
  next_cmd=strtok_r(NULL,"|",&svptr); 

  while (cmd != NULL && nbPipe<MAX_PIPE ) {

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
      if (pipe(arrayForPipe) == -1){
        if(argc) fprintf(stderr,"Pipe failed");
        exit(1);
      }
      if(argc) fprintf(stderr,"(PID:%d) created pipe wr:%d rd:%d \n",pid,arrayForPipe[1],arrayForPipe[0]);
    }

    pid = fork();
    if (pid<0) { perror("Erreur fork"); exit(1); }

    if (pid==0) {
      //pid=getpid();
      if(argc) fprintf(stderr,"(PID:%d) next_cmd: %s \n",pid,next_cmd);
      if(argc) fprintf(stderr,"(PID:%d) cmd: %s \n",pid,cmd);

      if(argc) fprintf(stderr,"(PID:%d) reading from fd: %d\n",pid,lectureDuPipePred);
      if ( dup2(lectureDuPipePred,0) == -1){
        if(argc) fprintf(stderr,"Duplication failed");
        exit(1);
      }
      close(lectureDuPipePred);
      close(arrayForPipe[0]);

      if(next_cmd != NULL){
        if(argc) fprintf(stderr,"(PID:%d) writing to fd: %d\n",pid,arrayForPipe[1]);
        if ( dup2(arrayForPipe[1],1) == -1){
          if(argc) fprintf(stderr,"Duplication failed");
          exit(1);
        }
      	close(arrayForPipe[1]);
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
      close(arrayForPipe[1]);
      lectureDuPipePred = arrayForPipe[0];
      cmd=next_cmd;
      next_cmd=strtok_r(NULL,"|",&svptr); 
      nbPipe++;
    }
  }
  free(cmd);
  pid_t wait(int *status);
}
