#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#define NO_OF_THREADS 3


/*********************************************************************************
 * @author
 * Malcolm Liljedahl, Robin Fransson, Maximilian Petersson
 * @brief 
 * In this assignment, you will write a program that creates three threads.
 * These threads access a shared integer, called buffer, one at a time. The buffer
 * will initially be set to 0. Each thread should print its thread ID, process ID, and
 * the buffer’s current value in one statement, then increment the buffer by one.
 * Use a mutex to ensure this whole process is not interrupted. Have the threads
 * modify the buffer 15 times. When each thread is done, it should return the
 * number of times it modified the buffer to the main thread. 
 ********************************************************************************/

pthread_mutex_t lock;
pthread_t thread[3];
int buffer = 0;


void *anFunction(void *vargp) {
    
    pid_t pid = getpid();
    int instance_counter = 0;

    while(true) {
        pthread_mutex_lock(&lock);
        if(buffer == 15){
            pthread_mutex_unlock(&lock);
            break;
        }
        printf("TID: %d, PID: %d Buffer: %d\n", pthread_self() , pid ,  buffer);
        buffer++;
        pthread_mutex_unlock(&lock); 
        
        instance_counter++;
    }

    printf("\nTID: %d, worked on buffer, %d times", pthread_self(), instance_counter);

    pthread_exit(0);
}

void main() {
    
    pthread_mutex_init(&lock, NULL);
    
    int i, j = 0;

    while(i < NO_OF_THREADS) {
        pthread_create(&thread[i], NULL, anFunction, NULL);
        i++;
    }

    while(j < NO_OF_THREADS) {
        pthread_join(thread[j], NULL);
        j++;
    }

    printf("\nTotal buffer accesses: %d", (buffer));
}