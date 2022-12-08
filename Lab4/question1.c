#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
Write a program that implements the following disk-scheduling algorithms:
FCFS, SSTF, SCAN, C-SCAN, Look and C-Look. Your program will service
a disk with 5000 cylinders numbered 0 to 4999. The program will generate a
random series of 1000 cylinder requests and service them according to each of
the algorithms listed above. The program will be passed the initial position
of the disk head (as a parameter on the command line) and report the total
amount of head movement required by each algorithm.
*******************************************************************************/

#define CYLINDERS 5000
#define CYLINDER_REQUESTS 1000

int start = 0;

/**FCFS**/
int fcfs(int array[]) {
    int
        starting_index = array[start];
    long head_movement = 0; 

    for(int i = start; i < CYLINDER_REQUESTS; i++) {
        head_movement = head_movement + abs(array[i] - starting_index);
        printf("%ld\t %d\n", head_movement, abs(starting_index - array[i]));
        
    }

    for(int i = 0; i < start; i++) {
        head_movement = head_movement +  abs(starting_index - array[i]);
        printf("%ld\t %ld\n", head_movement, abs(starting_index - array[i]));
    }

    return head_movement;
}

void main(int argc, char *argv[]) {

    start = atoi(argv[1]);

    int array[CYLINDER_REQUESTS];

    for(int i = 0; i < CYLINDER_REQUESTS; i++) {

		array[i] = rand() % 5000;
	}

    fcfs(array);

    // printf("FCFS head movements: %d\n", fcfs(array));
    

}

