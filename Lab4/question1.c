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
int array[CYLINDER_REQUESTS];

/**Function that sorts arrays**/
int* insertion_sort() {

	int temp = 0;

	for (int i = 0; i < CYLINDER_REQUESTS; ++i) {
    for (int j = i + 1; j < CYLINDER_REQUESTS; ++j) {
        if (array[i] > array[j]) {
            temp =  array[i];
            array[i] = 	array[j];
            array[j] = temp;
        }
      }
    }

    return array;
}

/**FCFS**/
int fcfs(int array[]) {

    int starting_index = array[start];
    long head_movement = 0; 

    for(int i = start; i < CYLINDER_REQUESTS; i++) {
        head_movement = head_movement + abs(array[i] - starting_index);
    }

    for(int i = 0; i < start; i++) {
        head_movement = head_movement + abs(starting_index - array[i]);
    }

    return head_movement;
}

/**SSTF**/
int sstf(int array[]) {

	int 
    lower_index = start - 1, 
    higher_index = start + 1,
    lower_index_difference = 0, 
    higher_index_difference = 0,
    head_movement = 0, 
    total = CYLINDER_REQUESTS - 2, 
    new_head = start;

    array = insertion_sort();
	
	while( total >= 0 ) {

		lower_index_difference = abs(array[new_head] - array[lower_index]);
		higher_index_difference = abs(array[higher_index] - array[new_head]);

		if(lower_index_difference < higher_index_difference) {
			head_movement += lower_index_difference;
			new_head = lower_index;
			lower_index--;	
		} 
    else {
			head_movement += higher_index_difference;
			new_head = higher_index;
			higher_index++;
		}

		total--;
	}
	return head_movement;
}

void main(int argc, char *argv[]) {

  start = atoi(argv[1]);

  //Inserts random numbers into the array
  for(int i = 0; i < CYLINDER_REQUESTS; i++) {
    array[i] = rand() % 5000;
	}

  printf("FCFS head movements: %d\n", fcfs(array));
  printf("SSTF head movements: %d\n", sstf(array));
    
}

