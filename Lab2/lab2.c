#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

pthread_mutex_t lock;
pthread_t thread[3];
int buffer = 0;

int counter1 = 0;
int counter2 = 0;
int counter3 = 0;

void *anFunction(void *vargp) {

    pthread_mutex_lock(&lock);
    sleep(1);
    pthread_mutex_unlock(&lock);  
}

void main() {
    pid_t pid = getpid();
    int i, j = 0;


    while(i < 3) {
        pthread_create(&thread[i], NULL, anFunction, (void *)&thread[i]);
        i++;
    }
    while(j < 15) {
        printf("TID: %d, PID: %d, Buffer: %d\n", thread[j % 3] , pid,  buffer);
        if(thread[j % 3] == thread[0]) {
            counter1++;
        }
        if(thread[j % 3] == thread[1]) {
            counter2++;
        }
        if(thread[j % 3] == thread[2]) {
            counter3++;
        }
        j++;
        buffer++;
    }

    printf("TID: %d worked on the buffer %d times\n", thread[0], counter1);
    printf("TID: %d worked on the buffer %d times\n", thread[1], counter1);
    printf("TID: %d worked on the buffer %d times", thread[2], counter1);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    

    

    
}