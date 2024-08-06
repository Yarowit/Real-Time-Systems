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

    sem_unlink(SEM_A_NAME);
    sem_unlink(SEM_B_NAME);

	int i = 10;
	sem_t *sem_A;
	sem_t *sem_B;
	pid_t pid;
	int status;

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
    	case 0: // Child process
        	// Attempt to execute a program to interacts with semaphores
        	if (execlp("./p6-2", "p6-2", NULL) == -1) {
            	perror("execlp");
            	exit(EXIT_FAILURE);
        	}
        	break; // This break is technically unnecessary due to exit above
    	
		default: // Parent process
        	while (i) {
            	// Wait (decrement) on sem_A before entering the critical section
            	if (sem_wait(sem_A) == -1) {
                	perror("sem_wait on sem_A failed");
                	exit(EXIT_FAILURE);
            	}

            	// Critical section: simulate work by printing a message
            	printf("Robot A writes-------%d\n", i--);
				sleep(2);
            	// Signal (increment) sem_B to allow the child process to proceed
            	if (sem_post(sem_B) == -1) {
                	perror("sem_post on sem_B failed");
                	exit(EXIT_FAILURE);
            	}
        	}

        	// Wait for the child process to complete
        	wait(&status);
        	break;
	}

	// Clean up: close and unlink semaphores
	if (sem_close(sem_A) == -1) perror("sem_close sem_A failed");
	if (sem_unlink(SEM_A_NAME) == -1) perror("sem_unlink SEM_A_NAME failed");
	if (sem_close(sem_B) == -1) perror("sem_close sem_B failed");
	if (sem_unlink(SEM_B_NAME) == -1) perror("sem_unlink SEM_B_NAME failed");

	return 0;
}
