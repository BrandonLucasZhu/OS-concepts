#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "FinalArray.h"
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>

//Completed by Brandon Zhu and Aaron Leung 

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(int valWait);
static int semaphore_v(int valSig);

	int id1;
	int id2;
	int id3;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	int shmid;
	int total = 0; // This is used for the summation calculation of each row
	char buffer[BUFSIZ];
	pid_t pid1;
	pid_t pid2;
	int argArray[5]; //Array to take in arguments
	int test1[5] = {5,6,8,2,7};
	char debug;

//Comment semun IF <sys/sem.h> CONTAINS SEMUN
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

void swap(int *x, int *y) {
	int temp = *x;
	
	//swap using call by reference in the function
	*x = *y;
	*y = temp;
}

//Checks array if it is non-increasing order
int checkArray(int array[5]){
	for (int i = 0; i < 4; i++){
		if (array[i] < array[i+1]){
			return 0;
		}
	}
	return 1;
}


int main(){

	printf("Do you want to enter debug mode y/n ?\n");
	debug = getchar();

	//Prompt User to enter 5 distinct values
	printf("Please input 5 distinct values \n");

	for(int i = 0; i < 5; i ++){
    		printf("Parameter %d:",i+1);
		scanf("%d", &argArray[i]);
		if (argArray[i] == 0) { //Validate if input is an integer
			printf("Not an integer closing program.\n");
			exit(EXIT_FAILURE);
		}
  	}

    	shmid = shmget((key_t)8866, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid, (void *)0, 0);

	shared_stuff = (struct shared_use_st*)shared_memory;

	//Set shared memory array equal to argument case array
	for (int i = 0; i < 5; i++){
		shared_stuff->array_mod[i] = argArray[i];
	}

	// Failed to allocate shared memory quit program
	if (shared_memory == (void *)-1) {
		printf("shmat failed\n");
		exit(EXIT_FAILURE);
	}	

	//initialize three semaphore for three critical sections
	id1 = semget((key_t)3434, 1, 0666 | IPC_CREAT);
	id2 = semget((key_t)3535, 1, 0666 | IPC_CREAT);
	id3 = semget((key_t)3636, 1, 0666 | IPC_CREAT);
	
	
	//Declare semaphore structure and set the parameter value to 1
	union semun d;
	d.val = 1;
	
	//Check to see if semaphore declaration failed by setting semaphore val
	if(semctl(id1, 0, SETVAL, d) < 0){
		printf("semctl"); 
		exit(EXIT_FAILURE);
	}

	if(semctl(id2, 0, SETVAL, d) < 0){
		printf("semctl"); 
		exit(EXIT_FAILURE);
	}

	if(semctl(id3, 0, SETVAL, d) < 0){
		printf("semctl"); 
		exit(EXIT_FAILURE);
	}



	pid1 = fork();
	pid2 = fork();

	if(pid1 < 0){
		//failed to create child process
		perror("fork"); 
		exit(0);
	}

	//1st Child process only. This acts as "process 2" and requires 2 semaphores to enter critical section shown in the diagram in the assignment 2 pdf
	if (pid1 == 0 && pid2 > 0){
		int not_organized = 0;
		while (not_organized == 0){

			if (!semaphore_p(id1)){
				printf("semop p"); 
				exit(EXIT_FAILURE);
			}
			if (!semaphore_p(id2)){
				printf("semop p"); 
				exit(EXIT_FAILURE);
			}			
						
			
			
			if (shared_stuff->array_mod[1] < shared_stuff->array_mod[2]){
				//Pass address location of values from the shared memory and swap the values
				swap(&shared_stuff->array_mod[1], &shared_stuff->array_mod[2]);
				if (debug == 'y'){
					printf("Process 2: %d swapped with %d \n",shared_stuff->array_mod[2],shared_stuff->array_mod[1]);
				}
			}
			else{
				if (debug == 'y'){
					printf("Process 2: %d did not swap with %d \n",shared_stuff->array_mod[1],shared_stuff->array_mod[2]);
				}
			}
			
			not_organized = checkArray(shared_stuff->array_mod);			

			if (!semaphore_v(id1)){
				printf("semop v"); 
				exit(EXIT_FAILURE);
		   	}
			if (!semaphore_v(id2)){
				printf("semop v"); 
				exit(EXIT_FAILURE);
		   	}



		}
		
		exit(0);
	}
	//Second Child process. This acts as process 3 and requires two semaphores to enter CS
	else if (pid1 > 0  && pid2 == 0){
		int not_organized = 0;
		while (not_organized == 0){

			if (!semaphore_p(id2)){
				printf("semop p"); 
				exit(EXIT_FAILURE);
			}
			if (!semaphore_p(id3)){
				printf("semop p"); 
				exit(EXIT_FAILURE);
			}			
						
			
			
			if (shared_stuff->array_mod[2] < shared_stuff->array_mod[3]){
				//Pass address location of values from the shared memory and swap the values
				swap(&shared_stuff->array_mod[2], &shared_stuff->array_mod[3]);
				if (debug == 'y'){
					printf("Process 3: %d swapped with %d \n",shared_stuff->array_mod[3],shared_stuff->array_mod[2]);
				}	
			}
			else{
				if (debug == 'y'){
					printf("Process 3: %d did not swap with %d \n",shared_stuff->array_mod[2],shared_stuff->array_mod[3]);
				}
			}
			
			not_organized = checkArray(shared_stuff->array_mod);			

			if (!semaphore_v(id2)){
				printf("semop v"); 
				exit(EXIT_FAILURE);
		   	}
			if (!semaphore_v(id3)){
				printf("semop v"); 
				exit(EXIT_FAILURE);
		   	}

		}
		
		exit(0);
	}
	//Third child process
	else if (pid1 == 0 && pid2 == 0){
		int not_organized = 0;
		while (not_organized == 0){

			if (!semaphore_p(id3)){
				printf("semop p"); 
				exit(EXIT_FAILURE);
			}			
			
			
			if (shared_stuff->array_mod[3] < shared_stuff->array_mod[4]){
				//Pass address location of values from the shared memory and swap the values
				swap(&shared_stuff->array_mod[3], &shared_stuff->array_mod[4]);
				if (debug == 'y'){
					printf("Process 4: %d swapped with %d \n",shared_stuff->array_mod[4],shared_stuff->array_mod[3]);
				}
				
			}
			else{
				if (debug == 'y'){
					printf("Process 4: %d did not swap with %d \n",shared_stuff->array_mod[3],shared_stuff->array_mod[4]);
				}
			}
			
			not_organized = checkArray(shared_stuff->array_mod);			

			if (!semaphore_v(id3)){
				printf("semop v"); 
				exit(EXIT_FAILURE);
		   	}


		}
		
		exit(0);
	} 
	else{
		int not_organized = 0;
		
		while (not_organized == 0){

			if (!semaphore_p(id1)){  // This acts as a wait and the sleep acts as the long wait
				printf("semop p"); 
				exit(EXIT_FAILURE);
			}			

			if (shared_stuff->array_mod[0] < shared_stuff->array_mod[1]){
				//Pass address location of values from the shared memory and swap the values
				swap(&shared_stuff->array_mod[0], &shared_stuff->array_mod[1]);
				if (debug == 'y'){
					printf("Process 1: %d swapped with %d \n",shared_stuff->array_mod[1],shared_stuff->array_mod[0]);
				}
				
			}
			else{
				if (debug == 'y'){
					printf("Process 1: %d did not swap with %d \n",shared_stuff->array_mod[0],shared_stuff->array_mod[1]);
				}
			}

			not_organized = checkArray(shared_stuff->array_mod);


			if (!semaphore_v(id1)){  //This acts as the signal
				printf("semop v"); 
				exit(EXIT_FAILURE);
		   	}

		}
		
	}
	
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
		
	
	
	//Print Shared Memory
	for (int i = 0; i< 5; i++){
		printf("[%d]",shared_stuff->array_mod[i]);
		
	}
	printf("\n");

	//Print max value
	printf("Max value is %d \n", shared_stuff->array_mod[0]);

	//Print minimum value
	printf("Minimum value is %d \n", shared_stuff->array_mod[4]);

	//Print median value 
	printf("Median value is %d \n", shared_stuff->array_mod[2]);

	

	//Delete semaphore values
	del_semvalue();	
	
	//Detach and delete shared memory
	if (shmdt(shared_memory) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmct1(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	
}


static int set_semvalue(void){
	union semun sem_union;
	sem_union.val = 1;
	if (semctl(id1, 0, SETVAL, sem_union) == -1) return(0);
		return(1);
	if (semctl(id2, 0, SETVAL, sem_union) == -1) return(0);
		return(1);
	if (semctl(id3, 0, SETVAL, sem_union) == -1) return(0);
		return(1);
}


	
static void del_semvalue(void){
	union semun sem_union;
	if (semctl(id1, 0, IPC_RMID, sem_union) == -1)
	fprintf(stderr, "Failed to delete semaphore\n");
	if (semctl(id2, 0, IPC_RMID, sem_union) == -1)
	fprintf(stderr, "Failed to delete semaphore\n");
	if (semctl(id3, 0, IPC_RMID, sem_union) == -1)
	fprintf(stderr, "Failed to delete semaphore\n");
}


static int semaphore_p(int idSemNum){
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(idSemNum, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return(1);
}


static int semaphore_v(int idSemNum){
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(idSemNum, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
		return(0);
	}
	return(1);
}








