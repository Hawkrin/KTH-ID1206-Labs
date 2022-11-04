#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>

#define MAX_SIZE 100
#define MAX_NUM_MSG 10

void main()
{
    mqd_t mq;               // message queue
    struct mq_attr ma;      // message queue attributes
    int status = 0;
    int a = 5;
    int b = 0;
    char buffer[MAX_SIZE + 1];
    char *write_msg = "hello";

    // Specify message queue attributes.
    ma.mq_flags = 0;                // blocking read/write
    ma.mq_maxmsg = 16;              // maximum number of messages allowed in queue
    ma.mq_msgsize = 100;    // messages are contents of an int
    ma.mq_curmsgs = 0;              // number of messages currently in queue

    // Create the message queue with some default settings.
    //mq = mq_open("/test_queue", O_RDWR | O_CREAT, 0700, &ma);
    mq = mq_open("/test_queue", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &ma);

    // -1 indicates an error.
    if (mq == -1)
    {
        printf("Failed to create queue.\n");
        status = 1;
    }

    if (status == 0)
    {
        status = mq_send(mq, write_msg, sizeof(write_msg), 1);
    }

    if (status == 0)
    {
        //status = mq_receive(mq, (char *)(&b), sizeof(int), NULL);
        status = mq_receive(mq, buffer, MAX_NUM_MSG, NULL);
    }

    if ((status == 0) && (mq_close(mq) == -1))
    {
        printf("Error closing message queue.\n");
        status = 1;
    }

    if ((status == 0) && (mq_unlink("test_queue") == -1))
    {
        printf("Error deleting message queue.\n");
        status = 1;
    }

    // mq_close(mqd);
    // mq_unlink(my_mq);


    for(int i = 0; i < 100; i++) {
        printf("%c\n", buffer[i]);
    }
} 