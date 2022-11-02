#include <stdio.h> //Used for basic input/output stream
#include <dirent.h> //Used for handling directory files
#include <errno.h> //Used for EXIT codes and error handling
#include <stdlib.h>

/**
 * @brief 
 * 
 * @param dir 
 */
void _ls(const char *dir) {
	
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
		if (d->d_name[0] == '.') {
			continue;
        }

		//printf("%s  ", d->d_name); //prints the file name or the directory name followed by a \n sign.

		count++;
	}

	printf("\n%d", count);
}

/**
 * @brief The function will take arguments and code accordingly to the options provided.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, const char *argv[]) {



	_ls(".");

	return 0;
}
