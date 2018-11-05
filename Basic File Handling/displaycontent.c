#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>


int main (int argc, char* argv[]){

	int fd;
// checking the user input from Program2.c
    if(2 != argc)
    {
        printf("\n Usage : Enter a file name  \n");
        return 1;
    }

    errno = 0;
// open the file in readonly mode, if doesn't exist, create one
    fd = open(argv[1],O_CREAT|O_RDONLY,S_IRWXU);

// set filepath
	char* filepath = argv[1]; 
    if(-1 == fd)
    {
        printf("\n error when opening %s \n",filepath);
        perror("open");
        return 1;
    }

// check if the file exist
if(access(filepath, F_OK) != 0 ){
		printf("\n %s dosen't exist, exit... \n",filepath);
		return 1;
}


// check if the file is readable
    if(access(filepath, R_OK) != 0 ){
		printf("\n %s is not readable, exit... \n",filepath);
		return 1;
}

// buffer with maximum 200 characters
	const size_t buff_size = 200;
	char buffer[buff_size];

// we need to get the bytes we need to read so we don't read into garbage in the buffer, and it's what the read function returns
// ssize_t to handle large number as well as negative number where int and size_t can't
	ssize_t byte_to_read;

// while byte_to_read is greater than 0, write that much bytes into the console , and byte to read update each time we call read
// so it will decrement 200 each time
// checking in whileloop spend a lot of time, don't know how to fix that
	while((byte_to_read = read(fd,buffer,buff_size)) >0){
		if (byte_to_read == -1) {
			printf("error when reading %s \n",filepath);
			perror("read");
			return 1;
		}
// we use byte_to_read instead of buffer_size because at last read we might have less than 100 characters, and we don't we to write garbage in the buffer
		if (write(1,buffer,byte_to_read) == -1){
			printf("error when displaying %s \n",filepath);
			perror("write");
			return 1;
		}
	}

// close the file when done using
	if (close(fd)==-1){
		printf("\n error when closing %s \n",filepath);
        perror("close");
        return 1;
}

	return 0;







}