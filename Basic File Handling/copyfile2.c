#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>


int main (int argc, char* argv[]){

	int source_fd,destination_fd,i;
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


// setting up the buffer size as 100 as it's required by the problem
    const size_t buff_size = 100;
    char buffer[buff_size];

// open the source and destination file in readonly and writeonly mode correspondingly, S_IRWXU granted the user the ability to read and write thefile
// if the file doesn't exist
    source_fd = open(source,O_CREAT|O_RDONLY,S_IRWXU);
    destination_fd = open(destination,O_CREAT|O_WRONLY,S_IRWXU);

// check if the sourcefile is readable
    if(access(source, R_OK) != 0 ){
        printf("\n %s is not readable, exit... \n",source);
        return 1;
}

// check if the destination is writable
    if(access(destination, W_OK) != 0 ){
        printf("\n %s is not writable, exit... \n",destination);
        return 1;
}


// check if there is error on opening file
     if(-1 == source_fd || -1 == destination_fd )
    {
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return 1;
    }

    ssize_t byte_to_read;

// until byte to read become 0, we read from source file and write it to destination file
// byte to read update each time we called read and become 0 when we done reading the whole file
    while((byte_to_read = read(source_fd,buffer,buff_size)) >0){
        if (byte_to_read == -1) {
            printf("error when reading %s \n",source);
            perror("read");
            return 1;
        }
// using for loop and go through buffer, if we see 1, we change it to A
            for (i =0;i<buff_size;i++){
                if(buffer[i] == '1')
                    buffer[i] ='A';
            }
// we use byte_to_read instead of buffer_size because at last read we might have less than 100 characters, and we don't we to write garbage in the buffer
        if (write(destination_fd,buffer,byte_to_read) == -1){
            printf("error when displaying %s \n",destination);
            perror("write");
            return 1;
        }
// add XYZ to the end each time we finish writing, not checking it as we just used the write function.
        write(destination_fd,"XYZ",3);
    }


// close files as we no longer need them
    if (close(source_fd)== -1){
        printf("error occurs when closing %s \n", source);
        perror("close");
        return 1;
}
    if (close(destination_fd)== -1){
        printf("error occurs when closing %s \n", destination);
        perror("close");
        return 1;
}
    return 0;

}