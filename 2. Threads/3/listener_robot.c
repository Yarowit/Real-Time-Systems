
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
// #include "shmz.h"
#define SEM_A_NAME "/sem_a" // Semaphore for parent process synchronization
#define SEM_B_NAME "/sem_b" // Semaphore for child process synchronization
#define SEM_C_NAME "/sem_c" // Semaphore for child process synchronization

#include <string.h> // memcpy

#define BUFF_SIZE 10

int* createSharedMemory(size_t size) {
    int *sharedMemory;

    int shm_fd = shm_open("/my_shared_memory",  O_RDWR, 0666);
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
    // sem_unlink(SEM_A_NAME);
    // sem_unlink(SEM_B_NAME);

	int i = 10;
	sem_t *sem_A;
	sem_t *sem_B;
	sem_t *sem_C;
	pid_t pid;
	int status;

	char *shared = (char*)createSharedMemory(sizeof(char)* BUFF_SIZE);
	

	// Open or create named semaphores
	// sem_A is initialized to 1, allowing the parent process to proceed
	sem_A = sem_open(SEM_A_NAME, O_EXCL, 0666, 1);
	// sem_B is initialized to 0, meaning the child process must wait
	sem_B = sem_open(SEM_B_NAME, O_EXCL, 0666, 0);
	sem_C = sem_open(SEM_C_NAME, O_EXCL, 0666, 0);

	// Check for errors in semaphore creation/opening
	if (sem_A == SEM_FAILED || sem_B == SEM_FAILED || sem_C == SEM_FAILED) {
    	perror("Semaphore creation/opening failed");
    	// Cleanup if any semaphore was successfully opened before failure
    	if (sem_A != SEM_FAILED) sem_close(sem_A);
    	if (sem_B != SEM_FAILED) sem_close(sem_B);
    	if (sem_C != SEM_FAILED) sem_close(sem_C);
    	exit(EXIT_FAILURE);
	}
	
	// program-----------------------------------------------------

	while (1){
		if (sem_wait(sem_C) == -1) {
			perror("sem_wait on sem_A failed");
			exit(EXIT_FAILURE);
		}

		printf("printing message:\n");
		printf("%s\n",shared);
		sleep(1);

		if (sem_post(sem_A) == -1) {
			perror("sem_post on sem_B failed");
			exit(EXIT_FAILURE);
		}
		if(shared[0] == 'e'){
			printf("exited\n");
			break;
		}
	}
	
	// program-----------------------------------------------------

	return 0;
}