#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>


void print_binary(int num){
    // assuming 32-bit integer
    for (int i = 31; i >= 0; i--) {
        
        // calculate bitmask to check whether
        // ith bit of num is set or not
        int mask = (1 << i);
        
        // ith bit of num is set 
        if (num & mask)
            printf("1");        
        // ith bit of num is not set   
        else 
           printf("0");
    }
    printf("\n");
}


int * mask_LSB16(int num){
    int page_offset[2];
    page_offset[0] = num & 0x0000FF00;
    page_offset[1] = num & 0x000000FF;
    return page_offset;
}

int main() {
    
    unsigned int b1 = 0b10000000000000000000000000000000;

    print_binary(b1);
    print_binary(mask_MSB16(b1));

}

