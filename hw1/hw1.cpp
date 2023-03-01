#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int seats [2][50];
int turn = 0;
int remainingSeats = 100;


void * firstReserver(void *)
{
	while(remainingSeats != 0)
	{
		while (turn != 0) {}
		
		//Critical region starts
		
		
		bool reserved = false;
		cout <<"Agency 1 Entered Critical Region" << endl;
		int row = rand() % 2;
		int col = rand() % 50;
		
		//We need to check the remaing seats again, since while we were busy waiting, other thread might have booked the last place.
		//We also need to check if the seat is empty.
		if(remainingSeats != 0 && seats[row][col] == 0)
		{
			cout << "Seat Number " << (50*row)+col+1 << " is reserved by Agency 1" << endl;
			seats[row][col] = 1;
			remainingSeats--;
			reserved = true;
		}
		
		//If there is no remaing seats left and we couldn't make a reservetion, then we will print no seats left.
		if(remainingSeats == 0 && !reserved)
		{
			cout << "Agency 1 Exit Critical Region" << endl;
			cout << "No seats left\n";
		}
		//If we made a reservation
		else
		{
			cout << "Agency 1 Exit Critical Region" << endl << endl;
		}
		
		
		//Critical region ends
		turn = 1;
	}

}

//This is a copy-paste version of the firstReserver function.
//Only agency names are changed.
void * secondReserver(void *)
{
	while(remainingSeats != 0)
	{
		while (turn != 1) {}
		
		bool reserved = false;
		cout <<"Agency 2 Entered Critical Region" << endl;
		int row = rand() % 2;
		int col = rand() % 50;
		
		if(remainingSeats != 0 && seats[row][col] == 0)
		{
			cout << "Seat Number " << (50*row)+col+1 << " is reserved by Agency 2" << endl;
			seats[row][col] = 2;
			remainingSeats--;
			reserved = true;
		}
		
		if(remainingSeats == 0 && !reserved)
		{
			cout << "Agency 2 Exit Critical Region" << endl;
			cout << "No seats left\n";
		}
		else
		{
			cout << "Agency 2 Exit Critical Region" << endl << endl;
		}
		turn = 0;
	}

}

int main()
{
	//To have a random seed.
	srand(time(NULL));
	
	//Initiliaze the array.
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 50; j++)
		{
			seats[i][j] = 0;
		}
	}

	//Create the threads
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, firstReserver, NULL);
	pthread_create(&thread2, NULL, secondReserver, NULL);
	
	//In main thread, we're checking if plane is full.
	while(remainingSeats != 0){}
	
	//If full, then join the threads.
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	//Print the plane.
	cout << "Plane is full:" << endl;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 50; j++)
		{
			cout << seats[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}