#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>

int main (int argc, char* argv[]){

	int fd,destination_fd;
// checking the user input from Program2.c, now we need 3 arguments
    if(3 != argc)
    {
        printf("\n Usage : Enter two file name, first the file to be copied and second the destination  \n");
        return 1;
    }

    errno = 0;

// setting up the name variable
    char* source = argv[1];
    char* destination = argv[2];


// setting up the buffer
    const size_t buff_size = 200;
    char buffer[buff_size];

// open the source file in readonly mode
    fd = open(source,O_CREAT|O_RDONLY,S_IRWXU);
    if(fd == -1) printf("failed to open %s \n",source);

// check if the sourcefile is readable
    if(access(source, R_OK) != 0 ){
        printf("\n %s is not readable, exit... \n",source);
        return 1;
}

// open the destination file in write only mode, if file doesn't exist, create a new one with ablility to be write and read
    destination_fd = open(destination,O_CREAT|O_WRONLY,S_IRWXU);
    if(destination_fd == -1) printf("failed to open %s \n",destination);

// check if the destination is writable
    if(access(destination, W_OK) != 0 ){
        printf("\n %s is not writable, exit... \n",destination);
        return 1;
}

// while byte_to_read is greater than 0, write that much bytes into the destination file , and byte to read update each time we call read
// so it will decrement 200 each time
    ssize_t byte_to_read;
    while((byte_to_read = read(fd,buffer,buff_size)) >0){
        if (byte_to_read == -1) {
            printf("error when reading %s \n",source);
            perror("read");
            return 1;
        }
// we use byte_to_read instead of buffer_size because at last read we might have less than 100 characters, and we don't we to write garbage in the buffer
        if (write(destination_fd,buffer,byte_to_read) == -1){
            printf("error when displaying %s \n",destination);
            perror("write");
            return 1;
        }
    }

// close both file when we done copying
    if (close(fd)==-1){
        printf("\n error when closing %s \n",source);
        perror("close");
        return 1;
}
    if (close(destination_fd)==-1){
        printf("\n error when closing %s \n",destination);
        perror("close");
        return 1;
}

    return 0;

}