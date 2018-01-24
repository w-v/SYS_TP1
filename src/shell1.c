#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
  char str[20];
  fgets(str, sizeof str, stdin);
  cleanUp(str);
  printf("commmand is %s \n", str);
  pid_t pid;
  pid = fork();
  if (pid<0) { perror("Erreur fork"); exit(1); }
  if (pid==0) {
    int jj = execlp(str, str, NULL);
    if (jj != 0){
      printf("commande introuvable");
    }
  }
  else {
    pid_t wait(int *status);
  }


  return 0;
}
