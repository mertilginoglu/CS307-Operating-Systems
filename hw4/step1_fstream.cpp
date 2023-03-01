#include <iostream>
#include <fstream>
#include <string>
#include <time.h> 

using namespace std;

int main()
{
	int repeat;
	for(repeat = 0; repeat < 5; repeat++)
	{
		clock_t t; 
		t = clock(); 
		int count =  0;
		char ch;
		ifstream myfile("loremipsum.txt");
		while(myfile.get(ch))
		{
			if (ch == 'a')
				count++;			
		}
		printf("Count is: %d", count);
		myfile.close();
		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf(".It took %f seconds to execute \n", time_taken); 
	}
	return 0;
}