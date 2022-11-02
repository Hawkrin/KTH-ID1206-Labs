#include <stdio.h> //Used for basic input/output stream
#include <dirent.h> //Used for handling directory files
#include <errno.h> //Used for EXIT codes and error handling
#include <stdlib.h>

/**
 * @brief 
 * 
 * @param dir 
 * @param op_a specifies that whether we want to list all files in the directory(which are hidden) or only the unhidden files
 * @param op_l specifies that whether we want to list all files in the normal form without going to next line and by keeping the hidden files hidden or not.
 */
void _ls(const char *dir,int op_a, int op_l) {
	
	struct dirent *d; // dirent object
	DIR *dh = opendir(dir); //used for navigation
	int count = 0;

    //if the file in not present or readable
	if (!dh) {
		if (errno = ENOENT) {
			perror("Directory doesn't exist");
		}
		else {
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}

	//While the next entry is not readable we will print directory files
	while ((d = readdir(dh)) != NULL) {

		//If hidden files are found we continue
		if (!op_a && d->d_name[0] == '.') {
			continue;
        }

		printf("%s  ", d->d_name); //prints the file name or the directory name followed by a \n sign.

		if(op_l) { 
            printf("\n"); 
        }
		count++;
	}

	if(!op_l) {
	    printf("\n");
    }

	printf("%d", count);
}

/**
 * @brief The function will take arguments and code accordingly to the options provided.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, const char *argv[]) {


	int op_a = 0;
	int op_l = 0;

	_ls(".",op_a, op_l);

	return 0;
}
