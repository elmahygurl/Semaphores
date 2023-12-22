#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define N 9  //number of counter threads 
//#define t1 1 //time interval for monitoring
#define b 10 //byffer size

//ANSI escape codes for colorful console output
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"

int count=0;  //accessed by mMonitor and incremented by mCounters
sem_t  semCounter;
sem_t semBuffer;
int buffer[b];

int front = 0;
int rear = 0;

struct Node {
    int data;
    struct Node* next;
};

struct Node* head = NULL;

void enqueue(int value)
{    
    printf("\n%sWaiting to enter ENQUEUE critical section to write in buffer\n",KRED);
    sem_wait(&semBuffer);
    printf("\n%sIN CRITICAL SECTION of enqueue- writing in buffer\n",KRED);
    if ((rear + 1) % b == front) //full buffer -no place to write
    {       
        printf("\n%sMonitor thread: Buffer full!! DIscarding value=%d\n",KRED, value);
        sem_post(&semBuffer);
        return;
    }

    buffer[rear] = value;
    printf("\n%sExiting enqueue critical section after enqueing at index=%d\n",KRED,rear);
    rear = (rear + 1) % b;
    
    
    sem_post(&semBuffer);
}


int dequeue()
{
    printf("\n%sWaiting to enter critical section to read from buffer-dequeue\n",KYEL);
    sem_wait(&semBuffer);
    printf("\n%sIN CRITICAL SECTION of dequeue- reading from buffer\n",KYEL);
    
    if (front == rear)  //empty buffer - nothing to read
    {	
    	printf("\n%sCollector thread: nothing is in the buffer!",KYEL);
        sem_post(&semBuffer);
        return -1; 
    }

    int value = buffer[front];
    printf("\n%sCollector thread: reading from the buffer at position %d",KYEL,front);
    front = (front + 1) % b;
    printf("\n%sExiting dequeue critical section\n",KYEL);
    sem_post(&semBuffer);
    return value;
}




void* collectingg(void *args)  
{
    while (1)
    {
    	int randomProcessingTime = rand() % 3 + 1; //get random num between 1 and 3
        sleep(randomProcessingTime);
        int value = dequeue();         //dequeue values from buffer 
        if (value != -1)
        {
            printf("\n%s mCollector dequeued value: %d\n", KYEL, value);
        }
        
        //sleep(t1); //every time interval
    }
}



void* countingg(void *args)
{
    int thread_no = *(int *)args;
    printf("\n%s mCounter no %d tryna enter critical\n", KRED,thread_no);
    sem_wait(&semCounter);
    printf("\n%sDa5lna critical of mCOunter no %d\n", KYEL,thread_no);
    count++;
    printf("\n%s mCounter no %d exiting critical section with count=%d\n",KGRN,thread_no,count);
    free(args); 
    sem_post(&semCounter);

}


void* monitoringg(void *args)
{
	while(1){ 
	int t1 = rand() % 2 + 1; //get random num between 1 and 2
        sleep(t1); 	
	printf("\n%sMonitor thread: waiting to read counter\n",KBLU);
	
    	sem_wait(&semCounter); // Lock the counter
    	printf("\n%sDa5lna critical of mMonitor\n", KBLU);
    	int current_count = count;
    	count= 0; // reset count
    	

    	printf("\n%sMonitor thread: reading a count value of %d\n", KBLU,current_count); //exiting critical 
    	sem_post(&semCounter); // unlock the counter
    	
    	enqueue(current_count);  //into buffer 
    	printf("\n%sEnqued into buffer count=%d\n",KRED,current_count);
    	
	
 	}
}

void intHandler(int dummy)
{
    // set the noramal color back
    printf("%sExit\n", KNRM);
    //destroy semaphores
    sem_destroy(&semCounter);
    sem_destroy(&semBuffer);
    exit(0);
}


int main()
{
    signal(SIGINT, intHandler);
    pthread_t mMonitor, mCollector;
    pthread_t mCounter[N] ;
    
    sem_init(&semCounter, 0, 1);
    sem_init(&semBuffer, 0, 1);
    
    pthread_create(&mMonitor, NULL, monitoringg, NULL); //monitoring
    
    pthread_create(&mCollector, NULL, collectingg, NULL); 
    
    
    while(1){
    	
        for (int i=0; i<N; i++)
    	{
    	int r=rand()%3+1;
    	printf("random counter var sleep = %d",r);
	sleep(r);
		int *thread_no = malloc(sizeof(int));
       		*thread_no = i;
       		pthread_create(&mCounter[i],NULL,countingg, thread_no);
    	}
  
   	for (int i = 0; i < N; i++)
    	{
        pthread_join(mCounter[i], NULL); // Wait for mCounter threads to finish
    	}
  
 	}
 	
    return 0;

}
