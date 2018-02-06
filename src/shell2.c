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

int occurence(char* str, char jc){
  int res = 0;
  for (int i = 0; str[i] != '\0'; i++){
    if (str[i] == jc){
      res++;
    }
  }
  return res;
}

int main (void)
{
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
  pid_t pid;
  pid = fork();
  if (pid<0) { perror("Erreur fork"); exit(1); }
  if (pid==0) {
    int jj = execvp(params[0], params);
    if (jj != 0){
      printf("commande introuvable");
    }
  }
  else {
    wait(NULL);
  }


  return 0;
}
