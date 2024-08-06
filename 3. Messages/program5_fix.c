// Program 5: Emotion Controller
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define SHARED_MEMORY_SIZE 256
#define SHARED_MEMORY_NAME "/shared_emotion"
#define TERMINATION_STRING "depressed"

struct sharedStruct {
	sem_t mutex;
	char sharedEmotion[SHARED_MEMORY_SIZE - sizeof(sem_t)];
}; // we want the size of structure to equal SHARED_MEMORY_SIZE


int main() {
	// Create shared memory for the emotional state
	int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SHARED_MEMORY_SIZE);

	/// ***** we map only once, to the struct *****
	struct sharedStruct * shared = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	/// ***** now we can pass references, so there is no need to change rest of he code
	char  *sharedEmotion = (shared->sharedEmotion);
	sem_t *mutex = &(shared->mutex);
	/// *******************************************

	/// ***** don't initialize mutex, the sender has already initialized it *****
	// sem_init(mutex, 1, 0);

	char previousEmotion[SHARED_MEMORY_SIZE] = "";

	while (1) {
		/// ********** every loop we print a message and value of mutex **********
		printf("waiting\n");
		int value;
		sem_getvalue(mutex,&value);
		printf("Mutex value: %d\n", value);
		/// **********************
		///  now the message is only printed once, the mutex actually waits for the change
		/// **********************

    	// Acquire lock
    	sem_wait(mutex);

    	// Display the emotional state if it has changed
    	if (strcmp(sharedEmotion, previousEmotion) != 0) {
        	printf("Robot Display: Current emotional state: %s\n", sharedEmotion);
        	strcpy(previousEmotion, sharedEmotion);
    	}

    	// Release lock
    	sem_post(mutex);

    	// Check for termination condition
    	if (strcmp(sharedEmotion, TERMINATION_STRING) == 0) {
        	break;
    	}

    	// Simulate a delay in processing and display
    	sleep(2);
	}

	// Close resources
	sem_destroy(mutex);

	return 0;
}
