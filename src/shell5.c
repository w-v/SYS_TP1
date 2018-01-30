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


int main (void)
{
  fprintf(stderr,"commande:");

  char str[MAX_CHAR];
  fgets(str, sizeof str, stdin);
  cleanUp(str);

  int arrayForPipe[2];
  int k;
  int lectureDuPipePred = 0;
  int nbPipe = 0;

  pid_t pid=getpid();

  char* params[MAX_CHAR/2];
  char* cmd = (char *) malloc(sizeof(char)*MAX_CHAR);
  char* tmp;
  cmd = strtok(str,"|");
  tmp=strtok(NULL,"|"); 

  while (cmd != NULL && nbPipe<MAX_PIPE ) {

    fprintf(stderr,"\n(PID:%d) tmp: %s \n",pid,tmp);
    fprintf(stderr,"(PID:%d) cmd: %s \n",pid,cmd);

    k = 0;
    params[0] = strtok(cmd," ");
    fprintf(stderr,"(PID:%d) params[%d]: %s \n",pid,k,params[k]);
    while ( params[k] != NULL){ 
      k++;
      params[k] = strtok(NULL," ");
      fprintf(stderr,"(PID:%d) params[%d]: %s \n",pid,k,params[k]);
    };
    params[k] = NULL;


    if(tmp != NULL){
      if (pipe(arrayForPipe) == -1){
        fprintf(stderr,"Pipe failed");
        exit(1);
      }
    }
    fprintf(stderr,"(PID:%d) created pipe wr:%d rd:%d \n",pid,arrayForPipe[1],arrayForPipe[0]);

    pid = fork();
    if (pid<0) { perror("Erreur fork"); exit(1); }

    if (pid==0) {
      //pid=getpid();
      fprintf(stderr,"(PID:%d) tmp: %s \n",pid,tmp);
      fprintf(stderr,"(PID:%d) cmd: %s \n",pid,cmd);

      if(tmp != NULL){
        fprintf(stderr,"(PID:%d) writing to fd: %d\n",pid,arrayForPipe[1]);
        if ( dup2(arrayForPipe[1],1) == -1){
          fprintf(stderr,"Duplication failed");
          exit(1);
        }
      }

      fprintf(stderr,"(PID:%d) reading from fd: %d\n",pid,lectureDuPipePred);
      close(arrayForPipe[1]);
      if ( dup2(lectureDuPipePred,0) == -1){
        fprintf(stderr,"Duplication failed");
        exit(1);
      }
      close(lectureDuPipePred);

      fprintf(stderr,"(PID:%d) exec: %s \n",pid,params[0]);
      if (execvp(params[0], params) != 0){
        fprintf(stderr,"commande introuvable");
        exit(1);
      }
      
      exit(0);
    }
    else {
      //el padre
      lectureDuPipePred = arrayForPipe[0];
      cmd=strtok(tmp+1,"|");
      tmp=strtok(NULL,"|"); 
      nbPipe++;
    }
  }
  free(cmd);
  pid_t wait(int *status);
}
