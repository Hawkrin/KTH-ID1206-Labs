#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct  {
} pt_entry;

typedef struct array {
    int pages;
    pt_entry *pagetable;   
} array;



void bitExtracted(unsigned int number, unsigned int msB, unsigned int lsB)
{   
    msB = ((number >> 16) & 0x0000FF00);
    lsB = ((number >> 16) & 0x000000FF);
    //unsigned int masked = number & 0xFFFF0000;
//
    //unsigned int page = masked & 0xFF000000;
    //unsigned int offset = masked & 0x00FF0000;
//
    //page = page >> 16;
    //offset = offset >> 16;
    //int res = 0x00FF;
    //return (((1 << k) - 1) & (number >> (p - 1)));
}


int main() {


/**SPLIT OFFSET AND PAGE NUMBER**/
/****************************************/
    unsigned int numbers[] = {1, 256, 32768, 32769, 128, 65534, 33153, 62493};
    unsigned int ch = 52461;
    unsigned int page = 0;
    unsigned int offset = 0;

    for (int i = 0; i < 8; i++){ // 0111 1111 1000
        page = (numbers[i] & 0x0000FF00) >> 8;
        offset = (numbers[i] & 0x000000FF);
        // printf("Number %d\n", i);
        // printf("Page number: %d\n", page);
        // printf("Offset number: %d\n\n", offset);
    }
/***********************************************/


/**READING INPUT FROM TEXT FILE**/
/*********************************/
    FILE* ptr;
    char ch2;

    ptr = fopen("data/addresses.txt", "r");

    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    printf("content of this file are \n");

    do {
        ch2 = fgchar(ptr);
        printf("%c", ch2);

    } while (ch2 != EOF);
    fclose(ptr);
/************************************/   

/* READING FROM BINARY FILE*/
/************************************/
    FILE *fp;
    char c[] = "this is tutorialspoint";
    char buffer[100];

    /* Open file for both reading and writing */
    fp = fopen("file.txt", "w+");

    /* Write data to the file */
    fwrite(c, strlen(c) + 1, 1, fp);

    /* Seek to the beginning of the file */
    fseek(fp, 0, SEEK_SET);

    /* Read and display data */
    fread(buffer, strlen(c)+1, 1, fp);
    printf("%s\n", buffer);
    fclose(fp);
/************************************/


    return 0;
}

