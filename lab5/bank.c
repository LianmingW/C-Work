#include <stdio.h>
#include <stdlib.h>
#include "sem.h"
#include <sys/stat.h>  // Needed for the S_IRUSR S_IWUSR types.
#include <time.h>

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */

main()
{
	/***********************************************************************************************
	IPC_PRIVATE is a key provided by the IPC library to secure the semaphore, 1 is the number of semaphore being created, IPC_CREAT creates the
	semaphore if it doesn't exist, S_IRUSR AND S_IWUSR gives the user the write and read permission.
	***********************************************************************************************/
	int sem = semget(IPC_PRIVATE,1,IPC_CREAT|S_IRUSR|S_IWUSR);
	/*******************************************************************************************
	If failed to create the sem, we get -1 as result
	*******************************************************************************************/
	if (sem == -1){
		perror("Failed to create the semaphore");
		exit(1);
	}

	/*******************************************************************************************
	create the semaphore, initilize the semaphore to 1, so only one process could enter
	*******************************************************************************************/
	sem_create(sem,1);
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables
	int dad_wait=0,son1_wait=0,son2_wait=0;
	clock_t start, end;
	double time_elapsed = 0;
	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);
	
	//Create child processes that will do the updates
		if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=3;
		for(i=1;i<=N; i++)
		{
			/*******************************************************************************************
			Each time we enter the critical region, that is when processes try to open the shared file balance or attempt, we lock
			the region, so only one of the Dad and sons could manipulate the files in a given time
			P(sem) means lock

			semctl(sem,0,GETVAL) returns the value of the semaphore right now, so if it is zero, this means we could not enter the critical section
			Thus, although Dad expressed the interest to go in, other processes are using it, so we increment dad's wait time by 1.

			Also I want to measure the number of cycles process forced to wait to enter the CS, where start is the start cycle, end is the end cycle
			we store the total cycle elapsed before and after the P(sem) into the variable time_elapsed;
	*******************************************************************************************/
			if (semctl(sem,0,GETVAL) == 0){
					dad_wait++;
				}
			start = clock();
			P(sem);
			end = clock();
			time_elapsed += end - start;
			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%5);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */
			/*******************************************************************************************
			After finishes the job, we unlock the semaphore, so other people can use the file
	*******************************************************************************************/
			V(sem);
		}
		/*******************************************************************************************
			Print the time dad has been waited, we could not print this after the Dad process 
			exit, as the global section are not shared between process, but each process has their own copy
	*******************************************************************************************/
		printf("Dad has been forced to wait %d times!\n",dad_wait);
		printf("Dad has been forced to wait %f cycles!\n",time_elapsed);
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				/*******************************************************************************************
				Similar procedure
	*******************************************************************************************/
				if (semctl(sem,0,GETVAL) == 0){
					son1_wait++;
				}
				start = clock();
				P(sem);
				end = clock();
				time_elapsed += end - start;
				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
						flag = TRUE;
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("Poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}
				/*******************************************************************************************
				Here is the end of the while loop, so no matter what condition we are in, the code will always reach
				here, as there are no exit or exec function being called in the while loop
	*******************************************************************************************/
				V(sem);
			}
			/*******************************************************************************************
				Print the number of time son1 has waited for other program.
	*******************************************************************************************/
			printf("Son1 has been forced to wait %d times! \n",son1_wait);
			printf("Son1 has been forced to wait %f cycles! \n",time_elapsed);
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					if (semctl(sem,0,GETVAL) == 0){
					son2_wait++;
						}
					start = clock();
					P(sem);
					end = clock();
					time_elapsed += end - start;
					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
							flag1 = TRUE;
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("Poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}
					V(sem);
				}
				printf("Son2 has been forced to wait %d times! \n",son2_wait);
				printf("Son2 has been forced to wait %f cycles! \n",time_elapsed);

			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
				// Remove the semaphore after done the updates.
				semkill(sem);
				// Number of attempts left 
				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				printf("Number of attempts left: %d\n", N_Att);

			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}




