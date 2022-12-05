#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct pagetable PageTable;
struct pagetable{
    int table_pointer; 
    int table[256][3];
};

typedef struct translation_lookaside_buffer TLB;
struct translation_lookaside_buffer{
    int table_pointer; 
    int table[12][3];
};

void init_pt(PageTable* pt);
int lookup_page(PageTable *pt, int page);
int* mask_LSB16(int num);
void mask_store_LSB16(int num, PageTable* pt);
void print_pt(PageTable* pt);
void print_binary(int num);
void test_bitmask();


int main(int argc, char **argv){    
    unsigned char physical_memory[65536];

    struct pagetable pt;
    init_pt(&pt);

    FILE *address_file, *backing_store;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int iterator = 0;

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
    
    while ((read = getline(&line, &len, address_file)) != -1) {
        int *page_offset = mask_LSB16(atoi(line));
        /*
            SOME TLB...
        */
        int frame, physical_address;
        if((frame = lookup_page(&pt, page_offset[0])) != -1){
            printf("HIIIT");//DET ÄR ORIMLIGT!
            physical_address = frame + page_offset[1];
        }
        else{
            iterator++;
            physical_address = (256 * iterator) + page_offset[1];
            fread(&physical_memory[physical_address], 256, 1, backing_store);
            pt.table[page_offset[0]][0] = page_offset[0];
            pt.table[page_offset[0]][1] = page_offset[1];
            pt.table[page_offset[0]][2] = physical_address;
        }
        int temp_physical_address = pt.table[page_offset[0]][2];
        signed char value = physical_memory[temp_physical_address];
        printf("Logical address: %d\tPhysical address: %d\tValue: %d\n", atoi(line), temp_physical_address, value);

        //mask_store_LSB16(line, &pt);
    }
    //printf("table_pointer: %d\n", pt.table_pointer);


    fclose(address_file);
    fclose(backing_store);
    if (line)
        free(line);

    //print_pt(&pt);
}

void init_pt(PageTable *pt){
    pt->table_pointer = 0;

    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 3; j++){
            pt->table[i][j] = -1;
        }
    }
}

int lookup_page(PageTable *pt, int page){
    return pt->table[page][2];
}

int * mask_LSB16(int num){
    static int page_offset[2];
    page_offset[0] = (num & 0x0000FF00) >> 8;
    page_offset[1] = (num & 0x000000FF);
    return page_offset;
}

void mask_store_LSB16(int num, PageTable* pt){
    printf("num: %d, cp: %d\n", num, pt->table_pointer);
    pt->table[pt->table_pointer][0] = (num & 0x0000FF00) >> 8;
    pt->table[pt->table_pointer][1] = (num & 0x000000FF);
    pt->table_pointer++;
}

void print_pt(PageTable* pt){
    printf("---Printing Page Table---\n");
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 3; j++){
            if(j == 3)
                printf("[Empty]");
            else
                printf("%d\t", pt->table[i][j]);
        }
        printf("\n");
    }
    printf("---End of Page Table---\n");
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