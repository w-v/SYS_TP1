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
  printf("commande:");
  char str[20];
  fgets(str, sizeof str, stdin);
  cleanUp(str);
  char* params[MAX_CHAR/2];
  int arrayForPipe[2];
  int k;
  int lectureDuPipePred = 0;
  char* cmd = (char *) malloc(sizeof(char)*MAX_CHAR);
  cmd = strtok(str,"|");
  char* tmp;
  int nbPipe = 0;
  while (cmd != NULL && nbPipe<MAX_PIPE ) {

    tmp=strtok(NULL,"|"); 
    printf("\ntmp: %s \n",tmp);
    printf("cmd: %s \n",cmd);

    k = 0;
    params[0] = strtok(cmd," ");
    printf("params[%d]: %s ",k,params[k]);
    while ( params[k] != NULL){ 
      k++;
      params[k] = strtok(NULL," ");
      printf("params[%d]: %s ",k,params[k]);
    };
    params[k] = NULL;


    if (pipe(arrayForPipe) == -1){
      printf("Pipe failed");
      exit(1);
    }

    pid_t pid;
    pid = fork();
    if (pid<0) { perror("Erreur fork"); exit(1); }
    if (pid==0) {
      printf("TMP: %s \n",tmp);
      if(tmp != NULL){
        printf("writing to fd: %d",arrayForPipe[1]);
        if ( dup2(arrayForPipe[1],1) == -1){
          printf("Duplication failed");
          exit(1);
        }
      }
      close(arrayForPipe[1]);
      if ( dup2(lectureDuPipePred,0) == -1){
        printf("Duplication failed");
        exit(1);
      }
      close(lectureDuPipePred);
      if (execvp(params[0], params) != 0){
        printf("commande introuvable");
      }
      
      exit(0);
    }
    else {
      lectureDuPipePred = arrayForPipe[0];
      cmd=tmp;
      nbPipe++;
    }
  }
  free(cmd);
  pid_t wait(int *status);
}
