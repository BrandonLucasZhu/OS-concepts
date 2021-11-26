#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "twoDStruct.h"
#define MICRO_SEC_IN_SEC 1000000


// Completed by: Brandon Zhu and Aaron Leung (Part 2)

// Global variables
struct timeval start, end;
void *shared_memory = (void *)0;
struct shared_use_st *shared_stuff;
int shmid;
int total = 0; // This is used for the summation calculation of each row
char buffer[BUFSIZ];
pid_t pid;
int n;
int computation;
int l = 4;
int h = 4;


//Hardcoded Matrix
int M[4][4] = {{1,2,3,4},{5,6,7,8},{4,3,2,1},{8,7,6,5}};
int N[4][4] = {{1,3,5,7},{2,4,6,8},{7,3,5,7},{8,6,4,2}};

int A[2][2] = {{1,2},{5,6}};
int B[2][2] = {{1,3},{2,4}};

int X[6][6] = {{1,2,3,4,5,6},{5,6,7,8,5,6},{4,3,2,1,5,6},{8,7,6,5,5,6},{8,7,6,5,5,6},{3,2,1,5,5,6}};

int Y[6][6] = {{1,3,5,7,1,8},{2,4,6,8,9,3},{7,3,5,7,1,3},{8,6,4,2,2,3},{3,2,1,5,5,6},{3,2,1,5,5,6}};



//Function for running a set amount of processes
void oneprocess(int matrix1[l][h], int matrix2[l][h]){
	// This will create one child processe

	gettimeofday(&start, NULL);

	pid = fork();
	if (pid == 0){
	//Using child process here to run the matrix multiplication
		for (int i = 0; i < l; i++){
			printf("Child Process One: working with row number %d for Matrix Q \n",i);
			for(int j = 0;j < l; j++){
				for(int k = 0;k < l; k++){
					total += matrix1[i][k]*matrix2[k][j];
				}
				//Modify the initialize struct of the four by four array Q
				// in shared memory

				if (l == 4){
					shared_stuff-> Q1[i][j] = total;
				}
				else if (l == 2)
					shared_stuff-> Q2[i][j] = total;
				else if (l == 6) {
					shared_stuff-> Q3[i][j] = total;
				}
				// Need to reset each calculation since shared memory exists
				total = 0;
			}

		}
		gettimeofday(&end, NULL);
		printf("\nElapsed Time: %ld micro sec\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
				gettimeofday(&end, NULL);
		exit(0);
	}
	//When child process finishes executing here, parent process runs
	wait(NULL);





}


void twoprocess(int matrix1[l][h], int matrix2[l][h]){


	int half = h/2; //Figure out the halfway point at which the second child process must be made
	gettimeofday(&start, NULL);
	pid_t child1 = fork();
	if  (child1 == 0){
		for (int i = 0; i < half; i++){
			printf("Child Process One: working with row number %d for Matrix Q \n",i);
			for(int j = 0;j < l; j++){
				for(int k = 0;k < l; k++){
					total += matrix1[i][k]*matrix2[k][j];
				}
				if (l == 4){
					shared_stuff-> Q1[i][j] = total;
				}
				else if (l == 2)
					shared_stuff-> Q2[i][j] = total;
				else if (l == 6) {
					shared_stuff-> Q3[i][j] = total;
				}
				// Need to reset each calculation since shared memory exists
				total = 0;
			}

		}
		gettimeofday(&end, NULL);
		printf("\nChild Process One elapsed Time: %ld micro sec\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
				gettimeofday(&end, NULL);
		exit(0);
	}
	gettimeofday(&start, NULL);
	pid_t child2 = fork();
	if  (child2 == 0){
		for (int i = half; i < l; i++){
			printf("Child Process Two: working with row number %d for Matrix Q \n",i);
			for(int j = 0;j < l; j++){
				for(int k = 0;k < l; k++){
					total += matrix1[i][k]*matrix2[k][j];
				}
				if (l == 4){
					shared_stuff-> Q1[i][j] = total;
				}
				else if (l == 2){
					shared_stuff-> Q2[i][j] = total;
				}
				else if (l == 6) {
					shared_stuff-> Q3[i][j] = total;
				}
				// Need to reset each calculation since shared memory exists
				total = 0;
			}

		}
		gettimeofday(&end, NULL);
		printf("\n Child Process Two elapsed Time: %ld micro sec\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
				gettimeofday(&end, NULL);
		exit(0);
	}

	//When child process 1 and 2 finishes executing here, parent process runs
	// Need to wait statements for the two processes
	wait(NULL);
	wait(NULL);
}

void fourprocess(int matrix1[l][h], int matrix2[l][h]){

	// This is literally part 1 code... for using all four processes making a child process
	// after each row increment on i


	for(int i = 0; i < l ;i++){
		gettimeofday(&start, NULL);
		pid = fork();
		switch(pid){
			case -1:
				perror("fork failed");
				exit(0);
			case 0:
		    		printf("Child Process %d: working with row number %d for Matrix Q \n", i, i);

				for(int j = 0;j < l;j++){
					for(int k = 0;k < l;k++){
						total += matrix1[i][k]*matrix2[k][j];
					}
					if (l == 4){
					shared_stuff-> Q1[i][j] = total;
					}
					else if (l == 2){
						shared_stuff-> Q2[i][j] = total;
					}
					else if (l == 6) {
						shared_stuff-> Q3[i][j] = total;
					}
					// Need to reset each calculation
					total = 0;
				}
				// Before child process finishes print out time when it is about to finish
				gettimeofday(&end, NULL);
				printf("Child Process %d elapsed Time: %ld micro sec\n", i,((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
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
}


void printMatrix(int mat1[l][h]){
	for(int i = 0;i < l;i++){
		printf("| ");
		for(int j = 0;j < l ;j++){
			printf("%d ", mat1[i][j]);
		}
		printf("|\n");
	}
}



int main (int argc, char *argv[]){

	char process_val[1];
	char matrix_order[1];

  printf ("Please enter Order of Matrix to be multiplied to be run 2,4, or 6 \n");
    scanf ("%s", matrix_order);
  l = atoi(matrix_order);
  if (argc != 1){
    exit(EXIT_FAILURE);
  }

	printf ("Please enter number of processes wanting to be run 1,2, or 4 \n");
  	scanf ("%s", process_val);
	n = atoi(process_val);
	if (argc != 1){
		exit(EXIT_FAILURE);
	}

	// Set up and declare shared memory for Q
	shmid = shmget((key_t)3552, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

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

	if (n == 1){
    if(l==2){
      oneprocess(A, B);
    }
    else if(l==6){
      oneprocess(X, Y);
    }
    else{
      oneprocess(M, N);
    }

	}
	else if (n == 2){
		if(l==2){
			twoprocess(A, B);
		}
		else if(l==6){
			twoprocess(X, Y);
		}
		else{
			twoprocess(M, N);
		}
	}
	else if (n == 4){
		if(l==2){
			fourprocess(A, B);
		}
		else if(l==6){
			fourprocess(X, Y);
		}
		else{
			fourprocess(M, N);
		}
	}


	//Must place wait statements here cause the four processes must be running concurrently during the for loop computations
	//Analogy for this logic above can be tortoise in the hare racing eventually they will get to the end but exception for this case is not knowing which one will make it to the end first
	//"Finish Line" is the wait statements to tell the child processes to stop at this point


	// After all four child processes done, parent continues after this point

	// Print the three matrix's
	printf("Matrix M\n");
	if(l==2){
		printMatrix(A);
	}
	else if(l==6){
		printMatrix(X);
	}
	else{
		printMatrix(M);
	}
	printf("Matrix: N\n");
	if(l==2){
		printMatrix(B);
	}
	else if(l==6){
		printMatrix(Y);
	}
	else{
		printMatrix(N);
	}


	printf("Matrix Q \n");
	for(int i = 0;i < l;i++){
		printf("| ");
		for(int j = 0;j < l;j++){
			if(l==2){
				printf("%d ", shared_stuff-> Q2[i][j]);
				shared_stuff-> Q2[i][j] = 0; //Reset all values in shared memory to 0
			}
			else if(l==6){
				printf("%d ", shared_stuff-> Q3[i][j]);
				shared_stuff-> Q3[i][j] = 0; //Reset all values in shared memory to 0
			}
			else{
				printf("%d ", shared_stuff-> Q1[i][j]);
				shared_stuff-> Q1[i][j] = 0; //Reset all values in shared memory to 0
			}
		}
		printf("|\n");
	}

}
