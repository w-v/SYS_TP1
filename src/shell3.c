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
  printf("commande1:");
  char str[20];
  fgets(str, sizeof str, stdin);
  cleanUp(str);
  char* params [10];
  params[0] = strtok(str," ");
  int i = 0;
  while( params[i] != NULL){ 
    printf("%s \n", params[i]);
    i++;
    params[i] = strtok(NULL," ");
  };
  params[i] = NULL;

  printf("commande2:");
  char str1[20];
  fgets(str1, sizeof str1, stdin);
  cleanUp(str1);
  char* params1 [10];
  params1[0] = strtok(str1," ");
  i = 0;
  while( params1[i] != NULL){ 
    printf("%s \n", params1[i]);
    i++;
    params1[i] = strtok(NULL," ");
  };
  params1[i] = NULL;

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
    int jj = execvp(params[0], params);
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
    int jj = execvp(params1[0], params1);
//    int jj = execlp("wc", "wc", "-l", NULL);
    if (jj != 0){
      printf("commande introuvable");
    }
    pid_t wait(int *status);
    exit(0);
  }


  return 0;
}
