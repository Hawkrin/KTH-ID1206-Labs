#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(){
    
   	FILE *file;
	char *buffer;
	unsigned long fileLen;
	
	//Open file
	file = fopen("data/BACKING_STORE.bin", "r");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", "data/BACKING_STORE.bin");
		return -1;
	}
	
	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	printf("%d\n", fileLen);
	fseek(file, 0, SEEK_SET);
	
	//Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
                                fclose(file);
		return -1;
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);
	
	//Do what ever with buffer
	free(buffer);
    
	return 0;
}