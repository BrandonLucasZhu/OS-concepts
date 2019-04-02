#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>


struct department_group {
	long type;
	int latest_employ_num[20]; // Print latest employees added
	char department_with_nums[12]; //send back department name as well
};
