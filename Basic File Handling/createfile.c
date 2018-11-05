#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h>
#include <unistd.h>

int main(){
// Declare the file name, or can take user argument if we want, but now lets stay with destination.txt
	const char* filename = "destination.txt";

// First check if the file exist
	int returnval = access(filename,F_OK);

// If dosen't we tell the user that we are creating the file
	if(returnval < 0)
		printf("%s dosen't exist, creating file...\n",filename);

// O_CREAT allow open to create new file if the file doesn't exist yet S_IRWXU give user permisson to read or write, O_RDWR means we can both read and write to the file
	int fd = open(filename, O_CREAT | O_RDWR, S_IRWXU);

// Error occured when fd <0
	if (fd < 0) {
		// If returnval <0, we failed to create the file, we exit
		if(returnval < 0){
		printf("error in creating %s \n",filename);
		perror("creat");
		return 1;
	}	// otherwise, we failed to open the file
		else{
		printf("error in opening %s \n",filename);
		perror("open");
		return 1;
	}
	}

// No error occurs and if returnval < 0 we send a message that the file is created successfully
	else{
		if(returnval < 0)
		printf("%s created successefully \n",filename );

		printf("%s opened successefully \n",filename );
}

// Close the file when we done using it
 	int status = close(fd);

// Error message
 	if(status < 0){
 		printf("Failed to close %s \n",filename);
		perror("close");
		return 1;
	}
	else printf("%s closed \n",filename);

// Optional: Delete the file so we can create a new file next time
	//status = unlink(filename);

// Close the program
	return 0;


}