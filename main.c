#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define N 9
#define t1 0.5
//ANSI escape codes for colorful console output
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"

int count=0;  //accessed by mMonitor and incremented by mCounters
sem_t  semCounter;

void* countingg(void *args)
{
    int thread_no = *(int *)args;
    printf("\n%s mCounter no %d tryna enter critical\n", KRED,thread_no);
    sem_wait(&semCounter);
    printf("\n%sDa5lna critical of mCOunter no %d\n", KYEL,thread_no);
    count++;
    printf("\n%s mCounter no %d exiting critical section with count=%d\n",KGRN,thread_no,count);
    free(args); // Free the allocated memory
    sem_post(&semCounter);

}


void* monitoringg(void *args)
{

	while(1){  	
	printf("\n%smMonitor tryna enter critical section\n",KBLU);
    	sem_wait(&semCounter); // Lock the counter
    	printf("\n%sDa5lna critical of mMonitor\n", KYEL);
    	int current_count = count;
    	count= 0; // Reset the counter
    	printf("\n%sExiting critical section of mMonitor, count before=%d\n", KBLU,current_count);
    	sem_post(&semCounter); // Unlock the counter
	sleep(t1); 
 }
}

void intHandler(int dummy)
{
    // set the noramal color back
    printf("%sExit\n", KNRM);
    //destroy the semaphore
    sem_destroy(&semCounter);
    exit(0);
}


int main()
{
    signal(SIGINT, intHandler);
    pthread_t mMonitor, mCollector;
    pthread_t mCounter[N] ;
    sem_init(&semCounter, 0, 1);
    
    pthread_create(&mMonitor, NULL, monitoringg, NULL); //monitoring
    
    while(1){
		//sleep(0.9);
        for (int i=0; i<N; i++)
    	{
		int *thread_no = malloc(sizeof(int));
       		*thread_no = i;
       		pthread_create(&mCounter[i],NULL,countingg, thread_no);
    	}
   

   	for (int i = 0; i < N; i++)
    	{
        pthread_join(mCounter[i], NULL); // Wait for mCounter threads to finish
    	}
  
 	}
 	//pthread_join(mMonitor, NULL); 
    return 0;

}
