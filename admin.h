#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>


struct employer_info_sent {
	long type;
	int command; 
	char name_send[12];
	char department_send[12];
	int employ_num_send;
	int salary_send;
};


