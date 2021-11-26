#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "twoDStruct.h"


// Completed by: Brandon Zhu and Aaron Leung (Part 1)

// Global variables 
void *shared_memory = (void *)0;
struct shared_use_st *shared_stuff;
int shmid;
int total = 0; // This is used for the summation calculation of each row
char buffer[BUFSIZ];
pid_t pid;


//Hardcoded Matrix
int M[4][4] = {{1,2,3,4},{5,6,7,8},{4,3,2,1},{8,7,6,5}};
int N[4][4] = {{1,3,5,7},{2,4,6,8},{7,3,5,7},{8,6,4,2}};


int main (){
	
	// Set up and declare shared memory for Q
	shmid = shmget((key_t)8888, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid, (void *)0, 0);

	shared_stuff = (struct shared_use_st*)shared_memory;

	if (shared_memory == (void *)-1) {
		printf("shmat failed\n");
		exit(EXIT_FAILURE);
	}

	
	// This will create four child processes that will run concurrently
   	for(int i = 0; i < 4 ;i++){
		pid = fork();
		switch(pid){
			case -1:
				perror("fork failed");
				exit(0);
			case 0:
		    		printf("Child Process %d: working with row number %d for Matrix Q \n", i, i);

				for(int j = 0;j < 4;j++){
					for(int k = 0;k < 4;k++){
						total += M[i][k]*N[k][j];
					}
					//Modify the initialize struct of the four by four array Q
					// in shared memory
					shared_stuff-> Q[i][j] = total;
					// Need to reset each calculation
					total = 0;
				}
				exit(0);
			default:
				break;
		}
	}
	
	//Must place wait statements here cause the four processes must be running concurrently during the for loop computations
	//Analogy for this logic above can be tortoise in the hare racing eventually they will get to the end but exception for this case is not knowing which one will make it to the end first
	//"Finish Line" is the wait statements to tell the child processes to stop at this point
	wait(NULL);
	wait(NULL);
	wait(NULL);	
	wait(NULL);

	// Print the three matrix's 
	printf("Matrix M: \n");
	for(int i = 0;i < 4;i++){
		printf("| ");	
		for(int j = 0;j < 4;j++){
			printf("%d ", M[i][j]);
		}
		printf("|\n");			
	}
	
	printf("Matrix N: \n");
	for(int i = 0;i < 4;i++){
		printf("| ");	
		for(int j = 0;j < 4;j++){
			printf("%d ", N[i][j]);
		}
		printf("|\n");			
	}

	printf("Matrix Q \n");
	for(int i = 0;i < 4;i++){
		printf("| ");	
		for(int j = 0;j < 4;j++){
			printf("%d ", shared_stuff-> Q[i][j]);
		}
		printf("|\n");			
	}

}
