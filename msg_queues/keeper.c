#include <sys/msg.h>
#include "keeper.h"
#include "admin.h"
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#define SERVER_MQUEUE 1234
#define CLIENT_MQUEUE 4321

int server_fifo_fd, client_fifo_fd;
int read_res;
char client_fifo[256];
char *tmp_char_ptr;
static int serv_qid = -1;
static int cli_qid = -1;
struct employer_info_sent data;
struct employee enterpriseLog[100];
struct employee noMatching;
int checkServer;


int main() {

	key_t key; 
	int msgid; 
	int msgid2;

	strcpy(noMatching.name,"No user match");
	noMatching.type = 1;

	//Initialize keeper data structure of array 
	for(int i = 0; i < 100; ++ i ) {
		enterpriseLog[i].isEmpty = 0;
		strcpy(enterpriseLog[i].name,"");
		strcpy(enterpriseLog[i].department, "");
		enterpriseLog[i].employ_num = 0;
		enterpriseLog[i].salary = 0;
	}
	
	// msgget creates a message queue to receive the incoming message
	msgid = msgget((key_t)3800, 0666 | IPC_CREAT);
		
	// msgget creates a message queue to receive the sending message to admin
	msgid2 = msgget((key_t)2300, 0666 | IPC_CREAT);


	
	for (;;){  

		
		// msgrcv to receive message 
		msgrcv(msgid, &data, sizeof(data), 1, 0); 
	

		if (data.command == 1){
		  	data.command = 0;
			// display the message obtained
			printf("Data being recieved and stored is: \n %s \n %s \n %i \n %i \n", data.name_send, data.department_send, data.employ_num_send, data.salary_send);
			
			//Insert data into the entry database when the first empty array data is found
		  	for(int i = 0; i < 100; i++ ) {
				if (enterpriseLog[i].isEmpty == 0){
					strcpy(enterpriseLog[i].name,data.name_send);
					strcpy(enterpriseLog[i].department, data.department_send);
					enterpriseLog[i].employ_num = data.employ_num_send;
					enterpriseLog[i].salary = data.salary_send;
					enterpriseLog[i].isEmpty = 1;
					break; // break out of the loop
				}
			}
		}
		else if (data.command > 1 && data.command < 5){
			//For returning a check in matching the employee number
		  	data.command = 0;
			int datafound = 0;
			
			for(int i = 0; i < 100; i++) {
				if (enterpriseLog[i].employ_num == data.employ_num_send){
					enterpriseLog[i].type = 1;
					msgsnd(msgid2, &enterpriseLog[i], sizeof(enterpriseLog[i]), 0);
					datafound = 1;
					
				}
			}
			//Return no employee records name since no match has been found
			if (datafound == 0){
				msgsnd(msgid2, &noMatching, sizeof(noMatching), 0);
			}
			
		}
		else if (data.command == 5){
			//For returning a result for a matching name sent from the admin
			int size;
		  	data.command = 0;
			int datafound = 0;
			for(int j = 0; j < 100; j++ ) {
				int nameCheck = strcmp(enterpriseLog[j].name, data.name_send);
				if (nameCheck == 0){
					enterpriseLog[j].type = 1;
					size = sizeof(enterpriseLog[j])-sizeof(enterpriseLog[j].type);
					msgsnd(msgid2, &enterpriseLog[j],size, 0);
					datafound = 1;
					break;
				}
			}
			//Return no employee records name since no match has been found
			if (datafound == 0){
				msgsnd(msgid2, &noMatching, sizeof(noMatching), 0);
			}
			
				
		}
		//For returning all the employees within this department
		else if (data.command == 6){
			//For returning a result for a matching name sent from the admin
			int size;
		  	data.command = 0;
			int datafound = 0;
			for(int j = 0; j < 100; j++ ) {
				int nameCheck = strcmp(enterpriseLog[j].department, data.department_send);
				if (nameCheck == 0){
					enterpriseLog[j].type = 1;
					size = sizeof(enterpriseLog[j])-sizeof(enterpriseLog[j].type);
					msgsnd(msgid2, &enterpriseLog[j],size, 0);
					datafound = 1;
				}
			}
			//Return no employee records name since no match has been found
			if (datafound == 0){
				msgsnd(msgid2, &noMatching, sizeof(noMatching), 0);
			}
			
				
		}	
		else if (data.command == 7){
		  	data.command = 0;

			int datafound = 0;
			for(int i = 0; i < 100; i++ ) {
				if (enterpriseLog[i].employ_num == data.employ_num_send){
					enterpriseLog[i].type = 1;
					msgsnd(msgid2, &enterpriseLog[i], sizeof(enterpriseLog[i]), 0);
					datafound = 1;
				}
			}
			//Return no employee records name since no match has been found
			if (datafound == 0){
				msgsnd(msgid2, &noMatching, sizeof(noMatching), 0);
			}

			
							
		}
		
		fflush(stdin);
	}
	 
	msgctl(msgid, IPC_RMID, NULL);

	return 0; 
}
