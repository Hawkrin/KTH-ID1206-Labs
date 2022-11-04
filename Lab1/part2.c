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


/**************************************************************************************
In this part, you will work with message queues. You need to implement two
processes, such that the first process reads the content of a text file and passes it
to the second process through a message queue. Upon receipt of the file content,
the second process should count and print out the number of words in the file.
***************************************************************************************/

void main() {

    char *my_mq = "/mymq";
    char *write_msg = "hello";
    char buffer[MAX_SIZE + 1];
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
    mq_send(mqd, write_msg, strlen(write_msg), 1);
    mq_close(mqd);

    // /**RECIEVING**/
    mqd = mq_open(my_mq, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attributes);
    mq_receive(mqd, buffer, MAX_NUM_MSG, NULL);
    printf("Message: %s\n", buffer);
    mq_close(mqd);
    mq_unlink(my_mq);

    // if (mqd == -1) {
    //     printf("Failed to create queue.\n");
    //     status = 1;
    // }

    // if (status == 0){ 
    //     mq_send(mqd, write_msg, strlen(write_msg), 1); 
    //     status = mq_receive(mqd, buffer, MAX_NUM_MSG, NULL);  
    // }

    // if ((status == 0) && (mq_close(mqd) == -1))
    // {
    //     printf("Error closing message queue.\n");
    //     status = 1;
    // }

    // if ((status == 0) && (mq_unlink("test_queue") == -1))
    // {
    //     printf("Error deleting message queue.\n");
    //     status = 1;
    // }

    printf("Message: %s\n", buffer);


} 


