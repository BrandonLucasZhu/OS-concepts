#include <sys/msg.h>
#include "admin.h"
#include "keeper.h"
#define SERVER_MQUEUE 1234
#define CLIENT_MQUEUE 4321
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>


struct employer_info_sent data;

int statusOnServer; //Status for checking the server
struct employee keeperData;
int operation;
int operation1; //For choosing admin task
char op[10];
pid_t pid;

int main() {
	
	key_t key; 
	int msgid; 
	int msgid2;
	
	//Create a message queue. This message queue is for doing one of the employee check, delete or insert commands
	msgid = msgget((key_t)3800, 0666 | IPC_CREAT); 

	//This message queue is used to close the server
	msgid2 = msgget((key_t) 2300, 0666 | IPC_CREAT); 

	printf("Choose a command by entering the numbered command:\n");
	printf("1: Insert employee information\n");
	printf("2: Check an employee Name\n");
	printf("3: Check an employee department\n");
	printf("4: Check an employee salary\n");
	printf("5: Check an employee number\n");
	printf("6: Check empoyees of a department division\n");
	printf("7: Delete an employee record\n");
	printf("Any number not stated above will close Admin and Keeper\n");
	 
	scanf(" %s",op);

	operation = atoi(op);	
	fflush(stdin);			

	while (1){	

	
		if (operation == 1){
		//Code for inserting new employee information
			printf("Enter the name \n");
			scanf( "%s", &data.name_send);
			printf("Enter the department name \n");
			scanf( "%s", &data.department_send);
			printf("Enter the employer number \n");
			scanf( "%i", &data.employ_num_send);
			printf("Enter the employer salary\n");
			scanf( "%i", &data.salary_send);
	 	 	data.type = 1;  
			data.command = 1;
			
			// msgsnd to send message 
			msgsnd(msgid, &data, sizeof(data), 0); 
	  
			// display the message 
			printf("\nData being sent is: \n %s \n %s \n %i \n %i \n", data.name_send, data.department_send, data.employ_num_send, data.salary_send); 
			printf("Choose a command by entering the numbered command:\n");
			printf("1: Insert employee information\n");
			printf("2: Check an employee Name\n");
			printf("3: Check an employee department\n");
			printf("4: Check an employee salary\n");
			printf("5: Check an employee number\n");
			printf("6: Check empoyees of a department division\n");
			printf("7: Delete an employee record\n");
			printf("Any number not stated above will close Admin and Keeper\n");
			 
			scanf(" %s",op);

			operation = atoi(op);	
			fflush(stdin);
		}
		else if (operation == 2){
			
			
			//Code for checking someones name
			printf("Enter the employer number to find employee name\n");
			scanf( "%i", &data.employ_num_send);
			data.type = 1;  
			data.command = 2;

			// msgsnd to send message 
			msgsnd(msgid, &data, sizeof(data), 0);
			
			//receive checked info
			msgrcv(msgid2, &keeperData, sizeof(keeperData), 1, 0);
			printf("\nName: %s    ", keeperData.name);
			printf("Employee ID:  %i \n \n", keeperData.employ_num);
			printf("Choose a command by entering the numbered command:\n");
			printf("1: Insert employee information\n");
			printf("2: Check an employee Name\n");
			printf("3: Check an employee department\n");
			printf("4: Check an employee salary\n");
			printf("5: Check an employee number\n");
			printf("6: Check empoyees of a department division\n");
			printf("7: Delete an employee record\n");
			printf("Any number not stated above will close Admin and Keeper\n");
			 
			scanf(" %s",op);

			operation = atoi(op);	
			fflush(stdin);
			
		}	
		else if (operation == 3){
			//Finding department for the employee numbers
			printf("Enter the employee number to get the employee department\n");
			scanf( "%i", &data.employ_num_send);
			data.type = 1;  
			data.command = 3;

			// msgsnd to send message 
			msgsnd(msgid, &data, sizeof(data), 0);
			
			//receive checked info
			msgrcv(msgid2, &keeperData, sizeof(keeperData), 1, 0);
			printf("\nDepartment: %s ", keeperData.department);
			printf("Employee ID: %i \n \n", keeperData.employ_num);

			//Re-ask the admin to enter a command
			printf("Choose a command by entering the numbered command:\n");
			printf("1: Insert employee information\n");
			printf("2: Check an employee Name\n");
			printf("3: Check an employee department\n");
			printf("4: Check an employee salary\n");
			printf("5: Check an employee number\n");
			printf("6: Check empoyees of a department division\n");
			printf("7: Delete an employee record\n");
			printf("Any number not stated above will close Admin and Keeper\n");
			 
			scanf(" %s",op);

			operation = atoi(op);	
			fflush(stdin);
		
		}
		else if (operation == 4){
			//Find salary for the employee
			printf("Enter the employee number to get the salary of employee\n");
			scanf( "%i", &data.employ_num_send);
			data.type = 1;  
			data.command = 4;

			// msgsnd to send message 
			msgsnd(msgid, &data, sizeof(data), 0);
			
			//receive checked info
			msgrcv(msgid2, &keeperData, sizeof(keeperData), 1, 0);
			printf("\nSalary Of Employee: %i ", keeperData.salary);
			printf("Employee ID: %i \n \n", keeperData.employ_num);
				
			//Re-ask the admin to enter a command
			printf("Choose a command by entering the numbered command:\n");
			printf("1: Insert employee information\n");
			printf("2: Check an employee Name\n");
			printf("3: Check an employee department\n");
			printf("4: Check an employee salary\n");
			printf("5: Check an employee number\n");
			printf("6: Check empoyees of a department division\n");
			printf("7: Delete an employee record\n");
			printf("Any number not stated above will close Admin and Keeper\n");
			 
			scanf(" %s",op);

			operation = atoi(op);	
			fflush(stdin);
			
		}
		else if (operation == 5){
			
			pid = fork();
			
			if (pid == 0){
				printf("Enter the name of the employee in order to get their ID\n");
				scanf( "%s", &data.name_send);
				data.type = 1;  
				data.command = 5;
				msgid = msgget((key_t)3800, 0666 | IPC_CREAT); 

				//This message queue is used to close the server
				msgid2 = msgget((key_t) 2300, 0666 | IPC_CREAT); 

				// msgsnd to send message 
				msgsnd(msgid, &data, sizeof(data), 0);
				
				//receive checked info
				msgrcv(msgid2, &keeperData, sizeof(keeperData), 1, 0);
				printf("\nName of Employee: %s ", keeperData.name);
				printf("Employee ID: %i \n \n", keeperData.employ_num);
					exit(EXIT_SUCCESS);
			
			}
			wait(NULL);
			
			printf("Choose a command by entering the numbered command:\n");
			printf("1: Insert employee information\n");
			printf("2: Check an employee Name\n");
			printf("3: Check an employee department\n");
			printf("4: Check an employee salary\n");
			printf("5: Check an employee number\n");
			printf("6: Check empoyees of a department division\n");
			printf("7: Delete an employee record\n");
			printf("Any number not stated above will close Admin and Keeper\n");
				 
			scanf(" %s",op);
			operation = atoi(op);	
			fflush(stdin);
				
			
		}
		else if (operation == 6){

			pid = fork();
			
			if (pid == 0){
				printf("Enter the department in order to get the correspondant ID to that department\n");
				scanf( "%s", &data.department_send);
				data.type = 1;  
				data.command = 6;

				// msgsnd to send message 
				msgsnd(msgid, &data, sizeof(data), 0);
				
				//receive checked info
				msgrcv(msgid2, &keeperData, sizeof(keeperData), 1, 0);
				printf("\nDepartment: %s ", keeperData.department);
				printf("Employee ID: %i \n \n", keeperData.employ_num);
				exit(EXIT_SUCCESS);
			}
			wait(NULL);
			

			//Re-ask for inputs
			printf("Choose a command by entering the numbered command:\n");
			printf("1: Insert employee information\n");
			printf("2: Check an employee Name\n");
			printf("3: Check an employee department\n");
			printf("4: Check an employee salary\n");
			printf("5: Check an employee number\n");
			printf("6: Check empoyees of a department division\n");
			printf("7: Delete an employee record\n");
			printf("Any number not stated above will close Admin and Keeper\n");
				 
			scanf(" %s",op);
			operation = atoi(op);	
			fflush(stdin);
			
		}
		else if (operation == 7){
			printf("Enter the employee number to delete that specific employee entry\n");
			scanf( "%i", &data.name_send);
			data.type = 1;  
			data.command = 7;

			// msgsnd to send message 
			msgsnd(msgid, &data, sizeof(data), 0);
			
			//receive checked info
			msgrcv(msgid2, &keeperData, sizeof(keeperData), 1, 0);
			printf("Department: %s ", keeperData.name);
			printf("Employee ID: %i \n", keeperData.employ_num);
			
		}
		
		
		
		
	}
	return 0; 

}

