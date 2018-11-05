#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc, char*argv[]){



if(2 != argc)
    {
        printf("\n Usage : Enter the directory to list content!  \n");
        return 1;
    }





	pid_t pid;
	pid = fork();
	// execvp takes in an array, so we initiallize it, -la shows all the hidden files as well as permission, last modified etc.., -lh make size readable by human.
	char* args[] = {"ls",argv[1],"-la","-lh",NULL};
	if (pid == 0){
		printf("Forked successfully! \n");
		execvp(args[0],args);
		perror("Execvp Failed!");
		exit(1);

	}

	else if (pid == -1){
		perror("Forkfailed \n");
	exit(1);
}









	else{
		// get child's pid, as child would have pid = 0, we print the real id using parent
		printf("Child's pid: %d \n",pid);
		// wait any child to die
		wait(NULL);
		printf("Child terminated!");
	}


}