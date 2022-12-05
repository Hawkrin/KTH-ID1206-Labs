#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h> 

#define NO_OF_PAGES 256         // Number of entries in the page table
#define PAGE_SIZE 256           // Page size in bytes
#define NO_OF_FRAMES 256        // Number of entries in the backing store?
#define FRAME_SIZE 256          // Frame size in bytes
#define NO_OF_TLB_ROW 16        // Number of rows in the TLB
#define NO_OF_TLB_COLUMNS 2     // Number of columns in the TLB

int main(int argc, char *argv[]) {
    
    FILE 
        *address_file, 
        *backing_store;
    char 
        physical_memory[NO_OF_FRAMES*FRAME_SIZE],
        *ch = malloc(10*sizeof(char)), 
        byte_value; 
    float 
        page_faults_rate,
        TLB_hit_rate;
    bool
        TLB_hit;
    int 
        logical_address, 
        physical_address, 
        page_number, 
        start,
        set_off,
        offset,
        page_faults = 0,
        TLB_hits = 0,
        empty_frame_index = -1,
        no_of_translations = 0, 
        frame_number,
        page_table[NO_OF_PAGES] = {[0 ... (NO_OF_PAGES - 1)] = -1 },  // -1 if free, 0 if allocated
        frame_table[NO_OF_PAGES] = {[0 ... (NO_OF_PAGES - 1)] = -1 }, // -1 if free, 0 if allocated   
        TLB[NO_OF_TLB_ROW][NO_OF_TLB_COLUMNS];

    memset(TLB, -1, sizeof(TLB[0][0]) * NO_OF_TLB_ROW * NO_OF_TLB_COLUMNS);

    address_file = fopen("data/addresses.txt", "r"); 
	backing_store = fopen("data/BACKING_STORE.bin", "r");
    if (address_file == NULL || backing_store == NULL){
        fprintf (stderr, "error: file open failed");
    }
    
    
    while(fgets(ch, 10, address_file)) {
        logical_address = atoi(ch);
        page_number = (logical_address & 0x0000ff00) >>  8;
        offset = logical_address & 0x000000ff;

         //TLB-hit, obtain frame number
        TLB_hit = false;
        for (int i = 0; i < NO_OF_TLB_ROW; i++) {
            if (TLB[i][0] == page_number) {
                physical_address = TLB[i][1];
                TLB_hit = true;
                TLB_hits++;
                break;
            }
        }

        //TLB-miss, pagetable-hit, obtain frame number	
        if(page_table[page_number]!= -1) {
            frame_number = page_table[page_number];
            physical_address = (frame_number * FRAME_SIZE) + offset;
            TLB[no_of_translations % NO_OF_TLB_ROW][0] = page_number;
            TLB[no_of_translations % NO_OF_TLB_ROW][1] = physical_address;
        }
        //TLB-miss, pagetable-miss, page-fault	
        else {
            page_faults++;
            empty_frame_index = -1;
            while (frame_table[empty_frame_index++] != -1){}
    
            //read page from backing store					
            if((size_t)backing_store != -1) {						
                
                start = NO_OF_PAGES * page_number;
                set_off = 0;

                fseek(backing_store, start, SEEK_SET);
                while((set_off < NO_OF_PAGES)) {
                    fgets(&byte_value, 2, backing_store);
                    physical_memory[empty_frame_index * NO_OF_FRAMES + set_off] = byte_value; 
                    set_off++;														
                }
            }
            else {
                fprintf(stderr, "error: backing store does not exist!");
                fclose(backing_store);
                fclose(address_file);	
                return 0;	
            }

            page_table[page_number] = empty_frame_index;	
            physical_address = (empty_frame_index * FRAME_SIZE) + offset;
            frame_table[empty_frame_index] = 0;
            TLB[no_of_translations % NO_OF_TLB_ROW][0] = page_number;
            TLB[no_of_translations % NO_OF_TLB_ROW][1] = physical_address;
        }
        byte_value = physical_memory[physical_address];
        no_of_translations++;
        memset(ch,0,sizeof(ch));
        printf("\nVirtual address: %d,  Physical address: %d, Value: %d", logical_address, physical_address, byte_value);

    }
    page_faults_rate = (float)page_faults / no_of_translations * 100.0;
    printf("\n\nPage fault rate: %.2f%c ", page_faults_rate, 37);

    TLB_hit_rate = (float) TLB_hits / no_of_translations * 100.0;
    printf("\nTLB hit rate: %.2f%c\n", TLB_hit_rate, 37);

    printf("\nNumber of translations: %d\n", no_of_translations);

    return 0;
}