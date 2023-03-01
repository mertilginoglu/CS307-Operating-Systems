#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
		char file_name[25] = "loremipsum.txt";
		char search = 'a';
		int count = 0;
		char ch;
		FILE *fp;

		fp = fopen(file_name, "r"); // read mode

		while(EOF != (ch = fgetc(fp)))
		{

			if(ch == search)
				count++;
		}

		printf("Count is: %d", count);
		fclose(fp);
		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf(".It took %f seconds to execute \n", time_taken); 
	}
	return 0;
}
