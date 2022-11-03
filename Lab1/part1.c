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

	/*************************THIS PART READS THE FILES(ls)************************/
	struct dirent *d; // dirent object
	DIR *dh = opendir("."); //used for navigation

    //if the file in not present or readable
	if (!dh) {
		if (errno = ENOENT) { perror("Directory doesn't exist"); }
		else { perror("Unable to read directory"); }
		exit(EXIT_FAILURE);
	}

	//While the next entry is not readable we will print directory files
	while ((d = readdir(dh)) != NULL) {

		//If hidden files are found we continue
		if (d->d_name[0] == '.') { continue; }

		//printf("%s\n", d->d_name); //prints the file name or the directory name followed by a \n sign.

	}
	/*************************THIS PART READS THE FILES(ls)************************/

	char* read_message[BUFFERSIZE];
	int fd[2];
	int pipe(int fd[2]);
	pid_t p = fork();

	switch (p) {
		case -1: // fork error
			printf("%d", "fork error");
			break;
		case 0: // Child
			close(fd[1]); // Close unused write end
			read(fd[0], read_message, BUFFERSIZE);
			printf("read %s", read_message);
		break;
		default: // Parent
			close(fd[0]); // Close unused read end
			write(fd[1], d->d_name, strlen(d->d_name) + 1);
		break;
		}

	return 0;
}
