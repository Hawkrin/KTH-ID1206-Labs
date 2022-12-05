#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct translation_lookaside_buffer TLB;
struct translation_lookaside_buffer{
    int table_pointer; 
    int table[12][2];
};

int main(){
    /*---Files init---*/
    FILE *address_file, *backing_store;

    char filepath0[50];
    strcpy(filepath0, "data/");
    strcat(filepath0, "addresses.txt");
    //strcat(filepath, &argv[1]);
    address_file = fopen(filepath0, "r");
    if (address_file == NULL){
        printf("ERROR: File '%s' failed to open.\n", filepath0);
        return -1;
    }
    char filepath1[50];
    strcpy(filepath1, "data/");
    strcat(filepath1, "BACKING_STORE.bin");
    backing_store = fopen(filepath1, "r");
    if (backing_store == NULL){
        printf("ERROR: File '%s' failed to open.\n", filepath1);
        return -1;
    }
    /*---End of Files init---*/

    unsigned char physical_memory[65536];
    int pagetable[256];
    for(int i = 0; i < 256; i++)
        pagetable[i] = -1;

    int frame = -256;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, address_file)) != -1) {
        //Mask the number
        int page = (atoi(line)>>8) & 255;
        int offset = atoi(line) & 255;
        /*
            Some TLB action in the future.
        */
        
        if(pagetable[page] != -1){
            //frame = pagetable[page];
        }
        else{
            frame += 256;
            fseek(backing_store, 256*page, SEEK_SET);
            fread(&physical_memory[frame], 256, 1, backing_store);
            pagetable[page] = frame;
        }
        printf("%d\t%d\n", page, offset);
        int physical_address = pagetable[page] + offset;
        signed char value = physical_memory[physical_address];

        printf("Logical address: %d\tPhysical address: %d\tValue: %d\n", atoi(line), physical_address, value);
    }

}