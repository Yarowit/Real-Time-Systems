#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

// Definitions for semaphore names, ensuring they are unique in the system
#define SEM_A_NAME "/sem_a" // Semaphore for parent process synchronization
#define SEM_B_NAME "/sem_b" // Semaphore for child process synchronization

int main(void) {
	int i = 10;
	sem_t *sem_A;
	sem_t *sem_B;

	// Open named semaphores
	// sem_A is used to signal "Robot A" to proceed
	// sem_B is used by "Robot B" to wait for "Robot A" to proceed
	sem_A = sem_open(SEM_A_NAME, O_CREAT, 0666, 1);
	sem_B = sem_open(SEM_B_NAME, O_CREAT, 0666, 0);

	// Check for errors in semaphore opening
	if (sem_A == SEM_FAILED || sem_B == SEM_FAILED) {
    	perror("Semaphore opening failed");
    	exit(EXIT_FAILURE);
	}

	while (i) {
    	// Wait on sem_B to be signaled by "Robot A" before proceeding
    	sem_wait(sem_B);
    	// Simulate the task of "Robot B" by printing a message
    	printf("Robot B writes-------%d\n", i--);
		sleep(2);
    	
// Signal sem_A to allow "Robot A" to proceed, synchronized order
    	sem_post(sem_A);
	}

	// Close and unlink the semaphores
	// Note: Typically, you wouldn't unlink a semaphore in a process that
	// doesn't know if others are done using it.
	// Unlinking is better done by a coordinating process or at system
	//startup/shutdown.
	// sem_close(sem_A);
	// sem_close(sem_B);

	// sem_close(sem_A);
	// sem_close(sem_B);
	// sem_unlink(SEM_A_NAME);
	// sem_unlink(SEM_B_NAME);


	return 0;
}
