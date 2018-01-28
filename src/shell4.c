#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * cleanUp(char* str){
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
  char* cmds [2];
  char* tmp = (char *) malloc(sizeof(char)*20);
  tmp = strtok(str,"|");
  int i = 0;
  while(tmp != NULL){ 
    printf("%s \n", tmp);
    cmds[i] = tmp;
    i++;
    tmp = strtok(NULL,"|");
  };
  free(tmp);

  char* tabcmds [2][10];
  for (int j = 0; i > j; j++){
    tabcmds[j][0] = strtok(cmds[j]," ");
    int k = 0;
    while( tabcmds[j][k] != NULL){ 
      printf("%s \n", tabcmds[j][k]);
      k++;
      tabcmds[j][k] = strtok(NULL," ");
    };
    tabcmds[j][k] = NULL;
  }

  int arrayForPipe[2];
  int retourDePipe = pipe(arrayForPipe);
  if (retourDePipe == -1){
    printf("Pipe failed");
    exit(1);
  }

  pid_t pid;
  pid = fork();
  if (pid<0) { perror("Erreur fork"); exit(1); }
  if (pid==0) {
    close(arrayForPipe[0]);
    int retourDeDup = dup2(arrayForPipe[1],1);//STDOUT_FILENO);
    close(arrayForPipe[1]);
    if (retourDeDup == -1){
      printf("Duplication failed");
      exit(1);
    }
    int jj = execvp(tabcmds[0][0], tabcmds[0]);
    //int jj = execlp("ls", "ls", "-l", NULL);
    if (jj != 0){
      printf("commande introuvable");
    }
  }
  else {
    close(arrayForPipe[1]);
    int retourDeDup = dup2(arrayForPipe[0],0);//STDIN_FILENO);
    close(arrayForPipe[0]);
    if (retourDeDup == -1){
      printf("Duplication failed");
      exit(1);
    }
    int jj = execvp(tabcmds[1][0], tabcmds[1]);
//    int jj = execlp("wc", "wc", "-l", NULL);
    if (jj != 0){
      printf("commande introuvable");
    }
    pid_t wait(int *status);
    exit(0);
  }


  return 0;
}
