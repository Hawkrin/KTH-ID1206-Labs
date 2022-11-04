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

    // Form the queue attributes
    struct mq_attr attributes;
    attributes.mq_maxmsg = MAX_NUM_MSG;
    attributes.mq_msgsize = MAX_SIZE;
    attributes.mq_flags = 0;
    attributes.mq_curmsgs = 0;

    /**RECIEVING**/
    mqd = mq_open(my_mq, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &attributes);
    mq_receive(mqd, buffer, MAX_NUM_MSG, NULL);
    printf("Message: %s\n", buffer);
    mq_close(mqd);
    mq_unlink(my_mq);

}

