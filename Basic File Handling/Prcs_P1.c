#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>





int main(int argc, char*argv[]){

// create destination1.txt and destination2.txt if they doesn't exist, with read and write and execute permissions (S_IRWXU)
	char* filename = "destination1.txt";
	char* filename2 = "destination2.txt";
	int fd = open(filename, O_CREAT | O_RDWR, S_IRWXU);
	int fd2 = open(filename2, O_CREAT | O_RDWR, S_IRWXU);


	close(fd);
	close(fd2);


}