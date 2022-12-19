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


void insertion_sort(int* arr) {

	int temp = 0;

	for (int i = 0; i < CYLINDER_REQUESTS; ++i) {
    for (int j = i + 1; j < CYLINDER_REQUESTS; ++j) {
        if (arr[i] > arr[j]) {
            temp =  arr[i];
            arr[i] = 	arr[j];
            arr[j] = temp;
        }
      }
    }
} 

/**FIRST COME FIRST SERVED(FCFS)**/
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

/**SHORTEST SEEK TIME FIRST(SSTF)**/
int sstf(int array[]) {

	int 
    lower_index = start - 1, 
    higher_index = start + 1,
    lower_index_difference = 0, 
    higher_index_difference = 0,
    head_movement = 0, 
    total = CYLINDER_REQUESTS - 2, 
    new_head = start;

    //array = insertion_sort();
	
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

/**SCAN**/
int scan(int array[]) {
  int 
    lower_index,
    higher_index,
    head_movement = 0, 
    current_index = -1;

  //array = insertion_sort();
  while (array[++current_index] < start) {}

  lower_index = current_index - 1;
  higher_index = current_index + 1;

  while (lower_index >= 0) {
    head_movement += abs(array[current_index] - array[lower_index]);
    current_index = lower_index--;
  }
  head_movement += abs(array[current_index]);

  while (higher_index < CYLINDER_REQUESTS) {
    head_movement += abs(array[higher_index] - array[current_index]);
    current_index = higher_index++;
  }
	return head_movement;
}

/**C-SCAN**/
int c_scan(int array[]) {
  int 
    start_index,
    higher_index,
    head_movement = 0, 
    current_index = -1;

  //array = insertion_sort();
  while (array[++current_index] < start) {}

  start_index = current_index--;

  while (current_index >= 0) {
    head_movement += abs(array[current_index] - array[current_index + 1]);
    current_index--;
  }
  head_movement += abs(CYLINDERS - 1 + array[current_index]);
  current_index = CYLINDER_REQUESTS - 1;
  while (current_index > start_index) {
    head_movement += abs(array[current_index] - array[current_index + 1]);
    current_index--;
  }
	return head_movement;
}

/**LOOK**/
int look(int array[]){
  int head_movement = 0;

  for(int head = start; head < CYLINDER_REQUESTS - 1; head++){
    head_movement += array[head + 1] - array[head];
  }

  head_movement += array[CYLINDER_REQUESTS-1] - array[start-1];

  for(int head = start - 1; head > 0; head--){
    head_movement += array[head] - array[head - 1];
  }

  return head_movement;
}

/**C_LOOK**/
int c_look(int array[]){
  int head_movement = 0;

  for(int head = start; head < CYLINDER_REQUESTS - 1; head++){
    head_movement += abs(array[head + 1] - array[head]);
  }

  head_movement += abs(array[CYLINDER_REQUESTS-1] - array[0]);

  for(int head = 0; head > start - 1 ; head++){
    head_movement += abs(array[head + 1] - array[head]);
  }

  return head_movement;
}



void main(int argc, char *argv[]) {

  if(argc < 2){
    printf("Missing start argument, expecting input from 0 to 4999\n");
    return;
  }

  start = atoi(argv[1]);

  
    
  //Inserts random numbers into the array
  for(int i = 0; i < CYLINDER_REQUESTS; i++) {
    array[i] = rand() % 5000;
	}

  printf("FCFS head movements: %d\n", fcfs(array));
  insertion_sort(array);
  //printf("FCFS(sorted) head movements: %d\n", fcfs(array));
  printf("SSTF head movements: %d\n", sstf(array));
  printf("SCAN head movements: %d\n", scan(array));
  printf("SCAN head movements: %d\n", c_scan(array));
  printf("LOOK head movements: %d\n", look(array));
  printf("CLOOK head movements: %d\n", c_look(array));
    
}

