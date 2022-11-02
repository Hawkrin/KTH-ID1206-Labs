#include <stdio.h> //Used for basic input/output stream
#include <dirent.h> //Used for handling directory files
#include <errno.h> //Used for EXIT codes and error handling
#include <stdlib.h> // standard library
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define _XOPEN_SOURCE 600

/**
 * @brief The function will take arguments and code accordingly to the options provided.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, const char *argv[]) {

	int pipe(int pipefd[2]);
	int fd1[2]; // Used to send input string from parent
    int fd2[2]; // Used to send back the number of lines.
	pid_t p;
	char input_str[100];

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

		printf("%s\n", d->d_name); //prints the file name or the directory name followed by a \n sign.
	}
	/*************************THIS PART READS THE FILES(ls)************************/

	
	if (pipe(fd1) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(fd2) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

	p = fork();

	if (p < 0) {
        fprintf(stderr, "fork Failed");
        return 1;
    }

	return 0;
}
