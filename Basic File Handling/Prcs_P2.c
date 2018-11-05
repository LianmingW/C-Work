#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>





int main(int argc, char*argv[]){

	int i;
	ssize_t byte_to_read;
// initialize file names
	char* filename = "destination1.txt";
	char* filename2 = "destination2.txt";
	char* source = "source.txt";
// open files, note we don't have O_CREAT, so the file must exist before we open it
	int fd = open(filename,  O_RDWR, S_IRWXU);
	int fd2 = open(filename2, O_RDWR, S_IRWXU);
	int source_fd = open(source, O_RDONLY);


	if( fd == -1 || fd2 == -1 || source_fd == -1){

		perror("Unable to create/open the file! Exit.....");
		exit(1);


	}

// 100 for destination1, 50 for destination2

	const size_t dest1_size = 100;
	const size_t dest2_size = 50;
	char buffer[dest1_size];
	char buffer2[dest2_size];

// while byte_to_read is greater than 0, which is the digit left to read (maximum 100, limited by the buffersize)
	while((byte_to_read = read(source_fd,buffer,dest1_size)) >0){

// change 1 to A 
        for (i =0;i<byte_to_read;i++){
                if(buffer[i] == '1')
                    buffer[i] ='A';
            }
// write to destination1
        write(fd,buffer,byte_to_read);

// maximum 50 for destination2
        byte_to_read = read(source_fd,buffer2,dest2_size);
// breakout from loop when byte to read is 0 or less
        if(byte_to_read <= 0){
        	break;
        }

// change 2 to B
        for (i =0;i<byte_to_read;i++){
                if(buffer2[i] == '2')
                    buffer2[i] ='B';
            }
// write to destination2
        write(fd2,buffer2,dest2_size);

}
// close the file when done copying
	close(fd);
	close(fd2);



	exit(0);

}