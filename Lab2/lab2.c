#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

pthread_mutex_t lock;
pthread_t thread[3];
int buffer = 0;

void *anFunction(void* thread) {

    pthread_mutex_lock(&lock);

    sleep(1);

    buffer++;

    pthread_mutex_unlock(&lock);  
}

void main() {
    pid_t pid = getpid();
    int j = 0;
    int o = 0;
    int counter = 0;

    while(j < 3) {

        pthread_create(&thread[j], NULL, anFunction, NULL);
        j++;
    }

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    
    while(o < 15) {
        printf("TID: %d, PID: %d,  Buffer: %d\n", thread[o % 3] , pid,  buffer);
        o++;
    }

    
}