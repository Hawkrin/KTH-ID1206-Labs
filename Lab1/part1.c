#include <stdio.h> //Used for basic input/output stream
#include <dirent.h> //Used for handling directory files
#include <errno.h> //Used for EXIT codes and error handling
#include <stdlib.h> // standard library
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define _XOPEN_SOURCE 600
#define BUFFERSIZE 200

// fd[0] read from the pipe, fd[1] write to the pipe
//int pipe(int fd[2]);

/**
 * @brief The function will take arguments and code accordingly to the options provided.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, const char *argv[]) {

	int fd[2], status, done = 0;
	int pipe(int fd[2]);
	char* listAll = "/bin/ls";
	char* countLines = "wc";
	pid_t p;
	p = fork();
	//char write_message[BUFFERSIZE];
	//char read_message[BUFFERSIZE];
	
	switch (p) {
		case -1: // fork error
			printf("fork error");
			break;
		case 0: // Child
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			execlp("ls", "ls", "/", (char *) NULL);
			break;
		default: // Parent
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			close(fd[1]);
			execlp("wc", "wc", "-l", (char *) NULL);
			break;
		}
	close(fd[0]);
	close(fd[1]);

	waitpid(-1, &status, 0);
}