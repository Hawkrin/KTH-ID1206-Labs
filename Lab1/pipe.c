#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void main(void){
    int fd[2];
    char buf;
    /*fd[0] kommer nu peka till läs delen av pipen och fd[1] pekar till skriv delen. 
    En pipe är enkel riktad. fd[1]--->fd[0]. */
    pipe(fd); 

    /*Only the shared memory segments are shared between the parent process and the newly forked 
    child process. Copies of the stack and the heap are made for the newly created process.
    När föräldern dör, dör dess barn. Barn exekveras parallelt med sin förälder.*/
    pid_t pid = fork(); 

    switch(pid){
        case -1: //Error forking
        break;
        case 0: //Child
            char *msg = "Hello World!";
            close(fd[0]); //Stäng läs porten (är det rätt att kalla den för port?) då child ska skriva. 
            write(fd[1], msg, strlen(msg)); //Argument: peka på den enden av pipen som är skriv, vad som ska skrivas, storleken på meddelandet. 
            close(fd[1]); //Stäng skriv porten. När både läs o skriv är stängda stängs den "virituella filen" (pipen) och child kan terminera. 
        break;
        default: //Parent
            close(fd[1]);
            while(read(fd[0], &buf, 1) > 0) //Läs 1 byte o spara i buf. read returnerar antal bytes lästa, i vårt fall returnerar den 1. Om den läser, skriv...
                write(STDOUT_FILENO, &buf, 1); //Skriv till output bufferten (terminalen).

            write(STDOUT_FILENO, "\n", 1); //Lägg till en endline.
            close(fd[0]);
        break;
    }

}