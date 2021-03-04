#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 3



int main(int argc, char **argv){

    char buf[BUFF_SIZE];
    int fd;
    int n;
	
	printf("--------------------- MonPG4 ---------------------\n\n");
	printf("Ce programme simule la commande 'cat' du terminal de commande.\n\n"
           "Parametres du programme : \n- Nom du fichier où l'on souhaite ecrire.\n"
           "\n\n");
	printf("--------------------------------------------------\n\n");
	
  if (argc > 2){
    fprintf(stderr, "Usage : %s [file.txt]\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  if (argc == 2){
    if((fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0600)) == 0){
        perror("Open ");
        exit(EXIT_FAILURE);
    }
    dup2(fd,STDOUT_FILENO);
  } else {
      fd =STDOUT_FILENO;
  }
  
  while((n=read(STDIN_FILENO,buf,BUFF_SIZE) )>0){
    if(write(fd,buf,n)==0){
        perror("Write ");
        exit(EXIT_FAILURE);
    }
  }

 
  close(fd);

  exit(EXIT_SUCCESS);
}
