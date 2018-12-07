#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

// Pthread mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Global variable
int ingredients[3] ={0};
const char* name[] = {"paper","tabacco","matches"};
// Number of cycles
const int agent_cycles = 10;



void *agent_action(void *parem){

	for (int i = 0; i< agent_cycles; i++){
	// lock the critical section
	pthread_mutex_lock(&mutex);
	int pick = rand()%3;
	// use random number to pick from 3 cases and increment the array storing the data respectively
	if (pick == 0){
		ingredients[0]++;
		ingredients[1]++;
		printf("%d: Agent put %s and %s on the table!\n",ingredients[3],name[0],name[1]);
	}
	else if (pick == 1){
		ingredients[0]++;
		ingredients[2]++;
		printf("%d: Agent put %s and %s on the table!\n",ingredients[3],name[0],name[2]);
	}
	else{
		ingredients[1]++;
		ingredients[2]++;
		printf("%d: Agent put %s and %s on the table!\n",ingredients[3],name[1],name[2]);
	}
	// Increment counter
	ingredients[3]++;
	// unlock
	pthread_mutex_unlock(&mutex);
	sleep(1);
}
	printf("Agent with id %ld has exited!\n",pthread_self());
	return NULL;
}



void *paper(void *param){
	while(1){
	// Lock when entering critical section
	pthread_mutex_lock(&mutex);
	// If it's the case we have tabacco and matches, consume them and smoke, else pass
	if (ingredients[1] == 1 && ingredients[2] == 1){
		ingredients[1]--;
		ingredients[2]--;
		printf("\tsmoker1 with paper smokes the cigarette!\n");
	}
	// if exit condition met, exit
	if (ingredients[3] >= agent_cycles){
		pthread_mutex_unlock(&mutex);
		break;
	}
	pthread_mutex_unlock(&mutex);
}
	printf("\tSmoker1 with id %ld has exited!\n",pthread_self());
	return NULL;
}


void *tabacco(void *param){
	while(1){

	pthread_mutex_lock(&mutex);
	if (ingredients[0] == 1 && ingredients[2] == 1){
		ingredients[0]--;
		ingredients[2]--;
		printf("\tsmoker2 with tabacco smokes the cigarette!\n");
	}
	if (ingredients[3] >= agent_cycles){
		pthread_mutex_unlock(&mutex);
		break;
	}
	pthread_mutex_unlock(&mutex);
}
	printf("\tSmoker2 with id %ld has exited!\n",pthread_self());
	return NULL;
}


void *matches(void *param){
	while(1){
	pthread_mutex_lock(&mutex);
	if (ingredients[0] == 1 && ingredients[1] == 1){
		ingredients[0]--;
		ingredients[1]--;
		printf("\tsmoker3 with matches smokes the cigarette!\n");
	}
	if (ingredients[3] >= agent_cycles){
		pthread_mutex_unlock(&mutex);
		break;
	}
	pthread_mutex_unlock(&mutex);
}
	printf("\tSmoker3 with id %ld has exited!\n",pthread_self());
	return NULL;
}



int main(){
	// Create thread ids
	pthread_t thread1,thread2,thread3,thread4;
	int agent, smoker1, smoker2, smoker3;
	// Lock the critical section where we creating the threads, don't want to get interrupted
	pthread_mutex_lock(&mutex);
	agent = pthread_create(&thread1,NULL,agent_action,NULL);
	smoker1 = pthread_create(&thread2,NULL,paper,NULL);
	smoker2 = pthread_create(&thread3,NULL,tabacco,NULL);
	smoker3 = pthread_create(&thread4,NULL,matches,NULL);
	// Unlock when done
	pthread_mutex_unlock(&mutex);
	// Wait for threads to finish
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	// Destroy the mutex as we don't need it any more
	pthread_mutex_destroy(&mutex);
	printf("All threads finished, exit....\n");
	exit(0);

}
