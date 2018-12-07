#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/wait.h>


#define SHSIZE 1024



int main(){
	// Number of time agent puts ingredients
	int agent_cycles = 10;
	/*******************************************************************************************
	Initialize variables
	*******************************************************************************************/
	pid_t smoker1, smoker2, smoker3;
	pid_t agent;
	// This is left for my first attempts of trying use amount stores in ingredients array to determine which smoker to smoke
	int ingredients[3] ={0};
	const char* name[] = {"paper","tabacco","matches"};
	// Our semaphore
	int lock = semget(IPC_PRIVATE,1,IPC_CREAT|S_IRUSR|S_IWUSR);
	int agent_lock = semget(IPC_PRIVATE,1,IPC_CREAT|S_IRUSR|S_IWUSR);
	int paper = semget(IPC_PRIVATE,1,IPC_CREAT|S_IRUSR|S_IWUSR);
	int tabacco = semget(IPC_PRIVATE,1,IPC_CREAT|S_IRUSR|S_IWUSR);
	int matches = semget(IPC_PRIVATE,1,IPC_CREAT|S_IRUSR|S_IWUSR);
	// For the usage of random
	srand(time(NULL));

	/*******************************************************************************************
	If failed to create the sem, we get -1 as result
	*******************************************************************************************/
	if (lock == -1 || agent_lock == -1 ||paper == -1 ||tabacco == -1 ||matches == -1 ){
		perror("Failed to create the semaphore");
		exit(1);
	}
	/*******************************************************************************************
	Create the semaphores with just lock initialize to 1, as if lock is 0, we would never be able to do anything
	0 means put into sleep
	*******************************************************************************************/
	sem_create(lock,1);
	sem_create(agent_lock,0);
	sem_create(paper,0);
	sem_create(tabacco,0);
	sem_create(matches,0);
	/*******************************************************************************************
	Share memory for interprocess communication
	*******************************************************************************************/
	int shmid;
	key_t key;
	char* shm;
	key = IPC_PRIVATE;
	shmid = shmget(key,SHSIZE,IPC_CREAT | 0666);
	if (shmid < 0){
		perror("shmget");
		exit(1);
	}

	// Fork agent
	agent = fork();
	if (agent == -1){
		perror("fork failed");
		exit(1);
	}
	// agent
	if (agent == 0){
		int pick;
		// Run for 10 times
		for(int j = 0;j<agent_cycles; j++){
			// P lock so no other process could interrupt 
			P(lock);
			sleep(1);
			printf("%d: ",j);
			// generate number from zero to two
			pick = rand()%3;
			int* ingredients = (int *)shmat(shmid,0,0);
			// If pick is 0, we wake up the smoker with matches
			if (pick == 0){
			ingredients[0]++;
			ingredients[1]++;
			printf("Agent puts %s and %s on the table!\n",name[0],name[1]);
			V(matches);
		}
			// If pick is 1, wake up smoker with tabacco
		else if (pick == 1){
			ingredients[0]++;
			ingredients[2]++;
			printf("Agent puts %s and %s on the table!\n",name[0],name[2]);
			V(tabacco);
		}
			// Else, wake up smoker with paper
		else{
			ingredients[1]++;
			ingredients[2]++;
			printf("Agent puts %s and %s on the table!\n",name[1],name[2]);
			V(paper);
		}
		// Put agents into sleep and increment the counter so smoker could terminate
		ingredients[3]++;
		shmdt((void*) ingredients);
		V(lock);
		P(agent_lock);
		}
		printf("Agent with pid %d exited!\n",getpid());
		// Wake up all smoker so they can exit
		V(paper);
		V(matches);
		V(tabacco);
		exit(0);
}
	else{
		smoker1 = fork();
		if (smoker1 == -1){
		perror("fork failed");
		exit(1);
	}
		//Smoker1 with paper
		if (smoker1 == 0){
			while(1){
			// sleep right away until being waked by the agent
			P(paper);
			P(lock);
			sleep(1);
			int* ingredients = shmat(shmid,0,0);
			// If agents puts 10 times , we exit
			if(ingredients[3] >= agent_cycles){
				if (ingredients[1]==1 & ingredients[2]==1){
					printf("Smoker1 with the paper successfully smokes the cigarette!\n");
				}
				printf("Smoker1 with pid %d exited!\n",getpid());
				shmdt((void*) ingredients);
				V(agent_lock);
				V(lock);
				exit(0);
			}
			ingredients[1]--;
			ingredients[2]--;
			shmdt((void*) ingredients);
			printf("Smoker1 with the paper successfully smokes the cigarette!\n");
			// wake up the agent
			V(agent_lock);
			V(lock);
		}
		}

	else{
		smoker2 = fork();
		if (smoker2 == -1){
		perror("fork failed");
		exit(1);
	}
	    // Smoker2 with tabacco
		if (smoker2 == 0){
			while(1){
			P(tabacco);
			P(lock);
			sleep(1);
			int* ingredients = shmat(shmid,0,0);
			if(ingredients[3] >= agent_cycles){
				if (ingredients[0]==1 & ingredients[2]==1){
					printf("Smoker2 with the tabacco successfully smokes the cigarette!\n");
				}
				printf("Smoker2 with pid %d exited!\n",getpid());
				shmdt((void*) ingredients);
				V(agent_lock);
				V(lock);
				exit(0);
			}
			ingredients[0]--;
			ingredients[2]--;
			shmdt((void*) ingredients);
			printf("Smoker2 with the tabacco successfully smokes the cigarette!\n");
			V(agent_lock);
			V(lock);
		}
		}

	else{
		smoker3 = fork();
		if (smoker3 == -1){
		perror("fork failed");
		exit(1);
	}
		// Smoker3 with matches
		if (smoker3 == 0){
			while(1){
			P(matches);
			P(lock);
			sleep(1);
			int* ingredients = shmat(shmid,0,0);
			if(ingredients[3] >= agent_cycles){
				if (ingredients[0]==1 & ingredients[1]==1){
					printf("Smoker1 with the matches successfully smokes the cigarette!\n");
				}
				printf("Smoker3 with pid %d exited!\n",getpid());
				shmdt((void*) ingredients);
				V(agent_lock);
				V(lock);
				exit(0);
			}
			ingredients[0]--;
			ingredients[1]--;
			shmdt((void*) ingredients);
			printf("Smoker3 with the matches successfully smokes the cigarette!\n");
			V(agent_lock);
			V(lock);
		}
		}
		// Wait for all of the process to die and clear up memory allocated
		wait(NULL);
		wait(NULL);
		wait(NULL);
		wait(NULL);
		semkill(lock);
		semkill(agent_lock);
		semkill(paper);
		semkill(tabacco);
		semkill(matches);
		printf("All processes exited, done\n");
		exit(0);





	}


	}





	}
















}