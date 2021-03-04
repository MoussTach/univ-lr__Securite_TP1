#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <errno.h>


int main(int argc, char** argv){
        
        int fdIn;
        int fdOut;
        int nbRead;
        char buffer[1000];
		
		printf("--------------------- MonPG3 ---------------------\n\n");
		printf("Ce programme permet de copier le contenu d'un fichier dans un autre.\n\n"
           "Parametres du programme : \n- Nom du fichier a lire.\n- Nom du fichier copié"
           "\n\n");
		printf("--------------------------------------------------\n\n");
        
        
        if (argc == 3)
        {
             fdIn = open(argv[1], O_RDWR, 0666); 
             fdOut = open(argv[2], O_WRONLY|O_CREAT, 0666);
             
             
             if (fdIn == -1 || fdOut == -1)
             {
                perror("open");
                exit(EXIT_FAILURE);
             }
             
             
             nbRead = read(fdIn, buffer, sizeof(fdIn));
             
             while (nbRead > 0)
             {
                write(fdOut, buffer, nbRead) ;
                nbRead = read(fdIn,buffer,sizeof(buffer)); 
             }
             
             close(fdIn);
             close(fdOut);
             exit(EXIT_SUCCESS);
             
        }
        else
        {
                fprintf(stderr,"Cette fonction necessite 2 arguments");
                exit(EXIT_FAILURE);  
        }
}
