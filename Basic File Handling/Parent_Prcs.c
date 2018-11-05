#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>


int main(int argc, char*argv[]){


pid_t pid,pid2;
// fork child 1;
pid = fork();
if(pid == 0){
	char* args[] = {"Prcs_P1",NULL};

	printf("Child1 running Prcs_P1....... \n");

	execv("./Prcs_P1",args);

	perror("Eexcv failed");

	exit(1);

}

if (pid == -1){
	perror("Forkfailed \n");
	exit(1);
}





else{
	wait(NULL);
	sleep(1);

	pid2 = fork();
	if (pid2 == 0){
	char* args[] ={"Prcs_P2",NULL};
	printf("Child2 running Prcs_P2......... \n");
	execv("./Prcs_P2",args);
	perror("Exacv failed");
	exit(1);


}
	else if (pid2 == -1){

		perror("Forkfailed \n");
	exit(1);
}

	








	wait(NULL);
	printf("Processes finished running, check the destination files! \n");
	exit(0);








}












	}



