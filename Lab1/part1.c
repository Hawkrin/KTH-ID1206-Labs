#include <stdio.h> /* Basic input/output stream */
#include <dirent.h> /* Handling directory files */
#include <errno.h> /* EXIT codes and error handling */
#include <stdlib.h> /* Standard library */
#include <unistd.h> /* STDOUT_FILENO, STDIN_FILENO */
#include <sys/wait.h> /* waitpid */

/***********************************************************************************************
 * @author
 * Malcolm Liljedahl, Robin Fransson, Maximilian Petersson
 * @brief 
 * The purpose of this part is to work with fork, exec, and wait to create new
 * processes and use pipe to communicate between parent/child processes. You
 * should implement a code to illustrate the following command: ls / | wc -l.
 * This command prints out the number of files in the root path: ls / shows the
 * files/directories in the root path, and its output will be piped through | to wc
 * -l, which counts the number of lines.
 *
 * Hint1: Use fork to make a child process. Then, the child process executes ls
 * /, passing the result (i.e., the list files/directories) through a pipe to the parent
 * process. The parent executes wc -l to print out the number of lines for the list
 * passed by the child.
 *
 * Hint2: You can use dup2 to redirect the output of the exec to the input descriptor made by pipe.
****************************************************************************************************/

/**
 * @brief The function perform the command: ls / | wc -l
 */
void main() {

	int fd[2], status, done = 0;
	pipe(fd);
	pid_t p = fork();

	switch (p) {
		case -1: // Fork Error
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