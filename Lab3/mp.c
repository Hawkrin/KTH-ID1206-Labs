#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

struct pagetable{
    int current_pointer; 
    int table[256][3];
};

typedef struct pagetable PageTable;

int* mask_LSB16(int num);
void mask_store_LSB16(int num, PageTable* pt);
void print_pt(PageTable* pt);
void print_binary(int num);
void test_bitmask();


int main(int argc, char **argv) {
    //printf("%s - %s\n", argv[0], argv[1]);
    
    struct pagetable pt;
    pt.current_pointer = 0;

    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char filepath[50];
    strcpy(filepath, "data/");
    strcat(filepath, argv[1]);
    fp = fopen(filepath, "r");
    if (fp == NULL){
        printf("ERROR: File failed to open.\n");
        return -1;
    }
        

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu: %s", read, line);
        //printf("%d\n", atoi(line));
        mask_store_LSB16(atoi(line), &pt);
    }

    fclose(fp);
    if (line)
        free(line);

    print_pt(&pt);
}

/*
    Strategi!
    Läs rad för rad från fil. 
    För varje rad, maska de 16 LSB, spara i page table.
*/

int * mask_LSB16(int num){
    static int page_offset[2];
    page_offset[0] = num & 0x0000FF00;
    page_offset[1] = num & 0x000000FF;
    return page_offset;
}

void mask_store_LSB16(int num, PageTable* pt){
    printf("num: %d, cp: %d", num, pt->current_pointer);
    pt->table[pt->current_pointer][0] = (num & 0x0000FF00);
    pt->table[pt->current_pointer][1] = (num & 0x000000FF);
    pt->current_pointer++;
}

void print_pt(PageTable* pt){
    for(int i = 0; i < pt->current_pointer; i++){
        for(int j = 0; j < 3; j++){
            if(j == 3)
                printf("[Empty]");
            else
                printf("%d\t", pt->table[i][j]);
        }
        printf("\n");
    }
}

void print_binary(int num){
    for (int i = 31; i >= 0; i--) {
        // calculate bitmask "(1 << i)" to check whether ith bit of num is set or not.
        if (num & (1 << i))
            printf("1");        
        else 
           printf("0");
    }
    printf("\n");
}

void test_bitmask(){
    unsigned int test_nums[] = {1, 256, 32768, 32769, 128, 65534, 33153};
    
    for(int i = 0; i < sizeof(test_nums)/sizeof(test_nums[0]); i++){
        printf("%d: ", test_nums[i]);
        int current = test_nums[i];
        print_binary(current);
        print_binary(mask_LSB16(current)[0]);
        print_binary(mask_LSB16(current)[1]);
        printf("\n");
    }
}