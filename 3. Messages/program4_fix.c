// Program 4: Emotion Sensor
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

	// Semaphore for synchronization
	sem_init(mutex, 1, 1);

	while (1) {
    	// Acquire lock
    	sem_wait(mutex);

    	// Simulate detecting emotional state (e.g., happy, sad, neutral)
    	printf("Enter the emotional state (type 'depressed' to exit): ");
		/// ***** we update the size of fgets not to overflow the shorter message memory *****
    	fgets(sharedEmotion, SHARED_MEMORY_SIZE - sizeof(sem_t), stdin);
    	strtok(sharedEmotion, "\n");  // Remove newline character

    	// Release lock
    	sem_post(mutex);

    	// Check for termination condition
    	if (strcmp(sharedEmotion, TERMINATION_STRING) == 0) {
        	break;
    	}

    	// Simulate a delay in sensor readings
    	sleep(2);
	}

	// Close and unlink resources
	sem_destroy(mutex);
	shm_unlink(SHARED_MEMORY_NAME);

	return 0;
}
