
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h> // for pid_t

// Definitions for semaphore names, ensuring they are unique in the system
#include "shmz.h"
#include <string.h> // memcpy

int* createSharedMemory(size_t size) {
    int *sharedMemory;

    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, size);
    sharedMemory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    return sharedMemory;
}

void detachAndRemoveSharedMemory(int* sharedMemory, size_t size) {
    if (munmap(sharedMemory, size) == -1) {
        perror("Shared memory unmap failed");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink("/my_shared_memory") == -1) {
        perror("Shared memory unlink failed");
        exit(EXIT_FAILURE);
    }
}

int main(){
    sem_unlink(SEM_A_NAME);
    sem_unlink(SEM_B_NAME);

	int i = 10;
	sem_t *sem_A;
	sem_t *sem_B;
	pid_t pid;
	int status;

	memo *shared = (memo*)createSharedMemory(sizeof(memo));
	
	shared->task = 10;

	// Open or create named semaphores
	// sem_A is initialized to 1, allowing the parent process to proceed
	sem_A = sem_open(SEM_A_NAME, O_CREAT | O_EXCL, 0666, 1);
	// sem_B is initialized to 0, meaning the child process must wait
	sem_B = sem_open(SEM_B_NAME, O_CREAT | O_EXCL, 0666, 0);

	// Check for errors in semaphore creation/opening
	if (sem_A == SEM_FAILED || sem_B == SEM_FAILED) {
    	perror("Semaphore creation/opening failed");
    	// Cleanup if any semaphore was successfully opened before failure
    	if (sem_A != SEM_FAILED) sem_close(sem_A);
    	if (sem_B != SEM_FAILED) sem_close(sem_B);
    	exit(EXIT_FAILURE);
	}
	
	// Forking to create a child process
	pid = fork();
	switch (pid) {
    	case -1: // Fork failed
        	perror("fork");
        	exit(EXIT_FAILURE);
    	case 0: // Child process - distance sensor
        	// Attempt to execute a program to interacts with semaphores
			while(shared->task > 0){
				if (sem_wait(sem_B) == -1) {
                	perror("sem_wait on sem_A failed");
                	exit(EXIT_FAILURE);
            	}
            	printf("Dectrementing task: %d",shared->task);
				(shared->task)--;
				printf(" -> %d\n", shared->task);

            	// Signal (increment) sem_B to allow the child process to proceed
            	if (sem_post(sem_A) == -1) {
                	perror("sem_post on sem_B failed");
                	exit(EXIT_FAILURE);
            	}
			}
			printf("returned\n");
			return 0;
		default: // Parent process - printer
			// printf("%d\n",access);
			while (1) {
            	// Wait (decrement) on sem_A before entering the critical section
            	if (sem_wait(sem_A) == -1) {
                	perror("sem_wait on sem_A failed");
                	exit(EXIT_FAILURE);
            	}

            	printf("Printing value: %d\n", shared->task);
				if(shared->task == 0) break;

            	// Signal (increment) sem_B to allow the child process to proceed
            	if (sem_post(sem_B) == -1) {
                	perror("sem_post on sem_B failed");
                	exit(EXIT_FAILURE);
            	}
        	}
        	// Wait for the child process to complete
			printf("waiting\n");
        	wait(&status);
        	break;
	}

	// Clean up: close and unlink semaphores
	if (sem_close(sem_A) == -1) perror("sem_close sem_A failed");
	if (sem_unlink(SEM_A_NAME) == -1) perror("sem_unlink SEM_A_NAME failed");
	if (sem_close(sem_B) == -1) perror("sem_close sem_B failed");
	if (sem_unlink(SEM_B_NAME) == -1) perror("sem_unlink SEM_B_NAME failed");
	
	detachAndRemoveSharedMemory((int*)shared, sizeof(memo));

	return 0;

}