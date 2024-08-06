// Definition of the access key to the common memory 
#define KEY_SHM ((key_t) 1001)

// Definition of keys for semaphores
// Definitions for semaphore names, ensuring they are unique in the system
#define SEM_A_NAME "/sem_a" // Semaphore for parent process synchronization
#define SEM_B_NAME "/sem_b" // Semaphore for child process synchronization
#define KEY 1

// Data structure to be shared in the common memory
typedef struct {
    int task;
} memo;