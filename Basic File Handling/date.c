#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(){

// fork a child
	pid_t pid;
	pid = fork();

	if(pid == 0) {
		printf("Forked successfully! \nCurrent time is:  \n");
		// /bin/date file path, date our argument, NULL terminate
		execl("/bin/date","date",NULL);
		// if execl failed, we print error and exit
		perror("Execl Failed");
		exit(1);
	}


	else{
		// get child's pid, as child would have pid = 0, we print the real id using parent
		printf("Child's pid: %d \n",pid);
		// wait any child to die
		wait(NULL);
		printf("Child terminated!");
	}

	return 0;


}