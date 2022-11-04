#include <stdio.h> //Used for basic input/output stream
#include <dirent.h> //Used for handling directory files
#include <errno.h> //Used for EXIT codes and error handling
#include <stdlib.h> // standard library
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_SIZE 100
#define MAX_NUM_MSG 10

void main() {

    char *my_mq = "/mymq";
    char *write_msg = "hello";
    char buffer[MAX_SIZE];
    mqd_t mqd;
    int status = 0;

    // Form the queue attributes
    struct mq_attr attributes;
    attributes.mq_maxmsg = MAX_NUM_MSG;
    attributes.mq_msgsize = MAX_SIZE;
    attributes.mq_flags = 0;
    attributes.mq_curmsgs = 0;

    /**SENDING**/
    mqd = mq_open(my_mq, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &attributes);

    if (mqd == -1) {
        printf("Failed to create queue.\n");
        status = 1;
    }

    mq_send(mqd, write_msg, strlen(write_msg), 1);
    mq_close(mqd);
    mq_unlink(my_mq);

}

