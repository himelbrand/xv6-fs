#include <stddef.h>
#include "types.h"
#include "user.h" 
#include "fcntl.h"

/* Sanity test for task 1*/
void
t1Test(void){
	printf(1,"running sanity test for task1\n");

	int fd;
	char data[1024];
	int numBytes;	//just to check the length
	// fills the data array with 7s
	memset(data, 7, 1024);

	fd = open("t1Sanity", O_CREATE | O_RDWR);
	numBytes = fd; //set initial position
	
	for(int i = 0; i < 1024; i++){
		numBytes += write(fd, data, sizeof(data));
		if (i == 5)
			printf(1,"Finished writing 6KB (direct)\n");
		if (i == 69)
			printf(1,"Finished writing 70KB (single direct)\n");
	}
	
	// checking the we indeed wrote 1MB data
	// NOTE: using ls after the test we can see the file
	//		 and its size in the file system of QEMU
	if(numBytes - fd == 1024*1024)
		printf(1,"Finished writing 1MB\ntest finished successfuly\n");
	else
		printf(1,"test FAILED!!\n");
}

int main(int argc, char* argv[]){
	printf(1,"**Sanity Tests**\n");
	t1Test();
	exit();
}