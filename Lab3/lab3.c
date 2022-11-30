#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define NO_OF_PAGES 256     // Number of entries in the page table
#define PAGE_SIZE 256       // Page size in bytes
#define NO_OF_FRAMES 256    // Number of entries in the backing store?
#define FRAME_SIZE 256      // Frame size in bytes

int main() {
    
    int logical_address, page_number, offset;
    char *ch = malloc(10*sizeof(char));
    int page_table[NO_OF_PAGES] = {[0 ... (NO_OF_PAGES - 1)] = -1 };
    FILE *address_file, *backing_store;
    
    printf("%d\n", page_table[100]);

    address_file = fopen("data/addresses.txt", "r"); 
	backing_store = fopen("data/BACKING_STORE.bin", "r");
    
    while(fgets(ch, 10, address_file)) {
        logical_address = atoi(ch);
        page_number = (logical_address & 0x0000ff00) >>  8;
        offset = logical_address & 0x000000ff;

        //printf("\nLogical Address: %d, Page Number: %d, Offset: %d", logical_address, page_number, offset);
    }
    
    return 0;
}