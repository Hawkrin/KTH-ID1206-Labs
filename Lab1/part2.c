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

int sender_process(struct mq_attr attributes, mqd_t mqd, char *my_mq ) {

    FILE *fp = fopen("text.txt", "r");
    char *read_buffer = malloc (sizeof *read_buffer * MAX_SIZE);
    size_t size = 0;
    int c;

    if (!fp) {  /* validate file open for reading */
        fprintf (stderr, "error: file open failed");
        return 1;
    }

    if (!read_buffer) { /* validate memory allocation succeeded */
        fprintf (stderr, "error: virtual memory exhausted.\n");
        return 1;
    }

    while ((c = fgetc (fp)) != EOF) {  /* for each char in file */
        read_buffer[size++] = c;       /* assign char to array */
    }

    if (fp != stdin) fclose (fp);    /* close file if not stdin */

    /**SENDING**/
    mqd = mq_open(my_mq, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &attributes);
    if (mqd == -1) {
        printf("Failed to create queue.\n");
        return 1;
    }
    mq_send(mqd, read_buffer, strlen(read_buffer), 1);
    mq_close(mqd);
    free (read_buffer);   /* free allocated memory */

    return 0;

}

int receiver_process (struct mq_attr attributes, mqd_t mqd, char *my_mq) {

    char buffer[MAX_SIZE + 1];
    char *write_msg = "";
    int i, words = 0;

    /**RECIEVING**/
    mqd = mq_open(my_mq, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attributes);
    mq_receive(mqd, buffer, MAX_SIZE, NULL);
    mq_close(mqd);

    mq_unlink(my_mq);     /* unlink queue */

    for(i = 0; buffer[i]; i++) {
        if(buffer[i] == 32)
        words++;
    }

    if( i > 0) { words++;}
    
    printf("The text: %s\n", buffer);
    printf("no of words in string = %d\n",words);

    return 0;

}



int main() {

    char *my_mq = "/mymq";
    mqd_t mqd;

         // Form the queue attributes
    struct mq_attr attributes;
    attributes.mq_maxmsg = MAX_NUM_MSG;
    attributes.mq_msgsize = MAX_SIZE;
    attributes.mq_flags = 0;
    attributes.mq_curmsgs = 0;
    
    sender(attributes, mqd, my_mq);

    receiver(attributes, mqd, my_mq);

    return 0;
} 


