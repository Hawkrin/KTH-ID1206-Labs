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
  int index = 0, head_movement = abs(array[index++] - start);
  for (; index < CYLINDER_REQUESTS - 1; index++) {
    head_movement += abs(array[index] - array[index + 1]);
  }
  return head_movement;
}

/**SSTF**/
int sstf(int array[]) {

	int
    lower_index,
    current_index = -1, 
    higher_index,
    lower_index_difference, 
    higher_index_difference,
    head_movement = 0, 
    head = start;

  array = insertion_sort();

  while (array[++current_index] < start) {}

  lower_index = current_index - 1;
  higher_index = current_index + 1;
	
	while (lower_index >= 0 || higher_index < CYLINDER_REQUESTS) {
    lower_index_difference = abs(array[lower_index] - head);
    higher_index_difference = abs(array[higher_index] - head);
    if (lower_index_difference < higher_index_difference && lower_index != -1) {
      head_movement += lower_index_difference;
      current_index = lower_index;
      head = array[lower_index--];
    }
    else {
      head_movement += higher_index_difference;
      current_index = higher_index;
      head = array[higher_index++];
    }
  }
	return head_movement;
}

int scan(int array[]) {
  int 
    lower_index,
    higher_index,
    head_movement = 0, 
    current_index = -1;

  array = insertion_sort();
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


int c_scan(int array[]) {
  int 
    start_index,
    higher_index,
    head_movement = 0, 
    current_index = -1;

  array = insertion_sort();
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

int look(int array[]) {
  int 
    start_index,
    higher_index,
    head_movement = 0, 
    current_index = -1;

  array = insertion_sort();
  while (array[++current_index] < start) {}

  start_index = current_index--;

  while (current_index >= 0) {
    head_movement += abs(array[current_index] - array[current_index + 1]);
    current_index--;
  }
  head_movement += abs(CYLINDERS - 1);
  while (current_index > start_index) {
    head_movement += abs(array[current_index] - array[current_index + 1]);
    current_index--;
  }
	return head_movement;
}

int c_look(int array[]) {
  return 0;
}

int main(int argc, char *argv[]) {

  start = atoi(argv[1]);

  //Inserts random numbers into the array
  for (int i = 0; i < CYLINDER_REQUESTS; i++) {
    array[i] = rand() % CYLINDERS;
	}

  printf("FCFS head movements: %d\n", fcfs(array));
  printf("SSTF head movements: %d\n", sstf(array));
  printf("SCAN head movements: %d\n", scan(array));
  printf("C-SCAN head movements: %d\n", c_scan(array));
  printf("LOOK head movements: %d\n", look(array));
  return 0;
}

