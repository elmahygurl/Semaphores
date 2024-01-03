# Semaphores
## Project overview 
This C program implements a multi-threaded counter system using pthreads and semaphores.

The system consists of three types of threads: mMonitor, mCollector, and mCounter.

The mMonitor thread monitors the counts generated by mCounter threads at regular intervals and enqueues them into a buffer.

The mCollector thread dequeues values from the buffer and processes them.
![image](https://github.com/elmahygurl/Semaphores/assets/97133077/0942467a-1d05-4892-92ea-0763e8f4313a)

## Objective:
1. Familiarizing with concurrent programming.
2. Handling races, synchronization, and deadlock conditions

## Sample Run 
![image](https://github.com/elmahygurl/Semaphores/assets/97133077/50c8d99f-1bdb-491b-ac22-9d5c0442e771)


