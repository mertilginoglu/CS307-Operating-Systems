#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size

int memoryTail = 0;
bool notFinished = true;

void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	node nextNode;
	nextNode.id = thread_id;
	nextNode.size = size;
	//Push the created node to queue. Lock first so we don't have problem.
	pthread_mutex_lock(&sharedLock);
	myqueue.push(nextNode);
	pthread_mutex_unlock(&sharedLock);
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	//Run until queue is finished.
	while(notFinished)
	{
		pthread_mutex_lock(&sharedLock); //Lock so that while accessing queue we don't have problem.
		while(!myqueue.empty())
		{
			//Get the front node and pop.
			node * currNode = new node();
			currNode = &myqueue.front();
			myqueue.pop();
			
			//If there is no memory available
			if((MEMORY_SIZE - memoryTail) < currNode->size)
			{
				thread_message[currNode->id] = -1;
			}
			//If there is memory available.
			else
			{
				thread_message[currNode->id] = memoryTail;
				memoryTail += currNode->size;
			}
			//Up the semaphore
			sem_post(&semlist[currNode->id]);
		}
		pthread_mutex_unlock(&sharedLock);
	}
}

void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	
	

	int size = (rand() % ((MEMORY_SIZE/6) - 1)) + 1; //In order to avoid getting 0.
	int * idPointer = (int *) id;
	my_malloc(*idPointer, size);
	//Block
	sem_wait(&semlist[*idPointer]);
	//Then fill the memory with id's or give an error prompt
	//If no memory available then -1.
	if(thread_message[*idPointer] == -1)
	{
		pthread_mutex_lock(&sharedLock);
		cout << "Thread " << *idPointer << ": Not enough memory." << endl;
		pthread_mutex_unlock(&sharedLock);
	}
	//If memory available, then fill.
	else
	{
	    
		for(int i = 0; i < size; i++)
		{
			memory[thread_message[*idPointer] + i] = '0' + (*idPointer);
		}
		/*
		pthread_mutex_lock(&sharedLock);
		cout << "Allocation done for thread " << *idPointer << "(Size: " << size << ")" << endl;
		pthread_mutex_unlock(&sharedLock);
		*/
	}
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
 // You need to print the whole memory array here.
    //cout << "dump_memory() starts" << endl;
	for(int i = 0; i < MEMORY_SIZE; i++)
		cout << memory[i];
}

int main (int argc, char *argv[])
 {

 	//You need to create a thread ID array here
	 int thread_ids[NUM_THREADS];
	 pthread_t threads[NUM_THREADS];

	 for (int i = 0; i < NUM_THREADS; i++)
	 {
		 thread_ids[i] = i;
		 pthread_t currThread;
		 threads[i] = currThread;
	 }

 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, thread_function, (void *) &thread_ids[i]);

 	//You need to join the threads

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

 	dump_memory(); // this will print out the memory
 	notFinished = false; //Make it false so that server thread stops running.
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }