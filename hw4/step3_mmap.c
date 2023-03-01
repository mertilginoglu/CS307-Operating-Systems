#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h> 


//https://www.tutorialspoint.com/cprogramming/c_file_io.htm
//https://www.studytonight.com/c/file-input-output.php#
//https://www.programiz.com/c-programming/c-file-input-output

// For compilation:
// gcc -o outputname.out filename.c 
// ./outputname.out

int main()
{
	int repeat;
	for(repeat = 0; repeat < 5; repeat++)
	{
		clock_t t; 
		t = clock(); 
		int fd = open ("loremipsum.txt", O_RDWR); /* Open the file for reading and writing. */
		struct stat s;
		size_t size;
		int status = fstat(fd, &s);
		size = s.st_size;
		int count = 0;
		
		char *ptr = mmap(0,size,
			PROT_READ|PROT_WRITE,MAP_SHARED,
			fd,0);
		if(ptr == MAP_FAILED){
			printf("Mapping Failed\n");
			return 1;
		}
		
		close(fd);
		int i;
		for(i=0; i < size ; i++)
		{
			if (ptr[size-i-1] == 'a')
				count++;
		}
		
		printf("Count is: %d", count);

		status = munmap(ptr, size);
		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf(". It took %f seconds to execute \n", time_taken); 
	}
	return 0;
}