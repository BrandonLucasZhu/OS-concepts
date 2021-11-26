#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
 

struct employee {
	long int type;
	char name[12];
	char department[12];
	int employ_num;
	int salary;
	int isEmpty;

};

