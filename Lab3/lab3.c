#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define NO_OF_PAGES 256     // Number of entries in the page table
#define PAGE_SIZE 256       // Page size in bytes
#define NO_OF_FRAMES 256    // Number of entries in the backing store?
#define FRAME_SIZE 256      // Frame size in bytes

int main(int argc, char *argv[]) {
    
    int logical_address, physical_Address, page_number, offset, frame_number;
    int page_fault = 0;
    char *ch = malloc(10*sizeof(char));
    int page_table[NO_OF_PAGES] = {[0 ... (NO_OF_PAGES - 1)] = -1 }; // -1 if free, 0 if allocated
    int frame_table[NO_OF_PAGES] = {[0 ... (NO_OF_PAGES - 1)] = -1 }; // -1 if free, 0 if allocated
    FILE *address_file, *backing_store;
    char byte_value;
    int empty_frame= - 1; 
    char physical_memory[NO_OF_FRAMES*FRAME_SIZE];
    int counter = 1;
    float percentage;

    address_file = fopen("data/addresses.txt", "r"); 
	backing_store = fopen("data/BACKING_STORE.bin", "r");
    
    while(fgets(ch, 10, address_file)) {
        logical_address = atoi(ch);
        page_number = (logical_address & 0x0000ff00) >>  8;
        offset = logical_address & 0x000000ff;

        //pagetable-hit, obtain frame number	
        if(page_table[page_number]!= -1) {
            frame_number = page_table[page_number];
            physical_Address = (frame_number * FRAME_SIZE) + offset;
        }
        //pagetable-miss, page-fault	
        else {
            page_fault++;
            empty_frame = -1;
            while (frame_table[++empty_frame] != -1){}
    
            //read page from backing-store into the available frame in the physical memory 					
            if((size_t)backing_store != -1) {						
                
                int d=0;
                int start = NO_OF_PAGES * page_number;

                fseek(backing_store, start, SEEK_SET);
                while((d < NO_OF_PAGES)) {
                    fgets(&byte_value, 10, backing_store);
                    physical_memory[empty_frame*offset] = byte_value; 
                    d++;														
                }
            }
            else {
                printf("Backing-Store Does not exist!");
                fclose(backing_store);
                fclose(address_file);	
                return 0;	
            }

            page_table[page_number] = empty_frame;	
            frame_table[empty_frame] = 0;
            physical_Address = (empty_frame * FRAME_SIZE) + offset;

        }
        byte_value = physical_memory[physical_Address];
        counter++;
        memset(ch,0,sizeof(ch));
        printf("\nVirtual address: %d,  Physical address: %d, Value: %d", logical_address, physical_Address, byte_value);

    }
    percentage = (float)page_fault / counter * 100.0;
    printf("\nPage fault rate: %.2f%c ", percentage, 37);	

    return 0;
}