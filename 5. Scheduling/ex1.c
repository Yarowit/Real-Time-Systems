#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

#include <unistd.h>

#define CLOCK_MONOTONIC 1

// Define struct for sensor and motor data
struct SensorMotorData {
    // Sensor data
    int sensor1;
    int sensor2;
    int sensor3;
    int sensor4;
    int sensor5;

    // Motor data
    int motor1;
    int motor2;
    int motor3;
    int motor4;
    int motor5;
} data;

// Define task periods in microseconds
#define m_sleep 10000
#define s_sleep 50000
#define t_sleep 100000

// Mutexes for thread synchronization
pthread_mutex_t s,m,t;

// Function to print the current time
void print_current_time() {
    struct timespec currentTime;
    char* message = "Nothing";
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    printf("CT: %ld %ld\n",currentTime.tv_sec,currentTime.tv_nsec);
    // TODO: Implement this function to print the current time in a human-readable format
}

// Motor control task function
void* motor_task(void* arg) {
    // TODO: Implement the motor control task
    while (1) {
        // Acquire motor control mutex
        pthread_mutex_lock(&m); 
        // Print current time
        printf("Motor: ");
        print_current_time();
        printf("%d %d %d %d %d\n",data.motor1,data.motor2,data.motor3,data.motor4,data.motor5);
        // Release motor control mutex
        pthread_mutex_unlock(&m); 
        // Sleep for task period
        usleep(m_sleep);
    }
    return NULL;
}

// Sensor data processing task function
void* sensor_task(void* arg) {
    // TODO: Implement the sensor data processing task
    while (1) {
        // Acquire sensor data mutex
        pthread_mutex_lock(&s);
        pthread_mutex_lock(&t);
        // Print current time
        printf("Sensor: ");
        print_current_time();
        data.sensor1 = rand()%60;
        data.sensor2 = rand()%60;
        data.sensor3 = rand()%60;
        data.sensor4 = rand()%60;
        data.sensor5 = rand()%60;
        // Release sensor data mutex
        pthread_mutex_unlock(&s);
        pthread_mutex_unlock(&t);
        // Sleep for task period
        usleep(s_sleep);
    }
    return NULL;
}

// Trajectory planning task function
void* trajectory_task(void* arg) {
    // TODO: Implement the trajectory planning task
    while (1) {
        // Acquire trajectory planning mutex
        pthread_mutex_lock(&t);
        pthread_mutex_lock(&s);
        pthread_mutex_lock(&m);
        // Print current time
        printf("Trajectory: ");
        print_current_time();
        data.motor1 = data.sensor1 - data.sensor5;
        data.motor2 = data.sensor1 + 2*data.sensor2;
        data.motor3 = data.sensor2 + 2*data.sensor3;
        data.motor4 = data.sensor4 + 4*data.sensor3;
        data.motor5 = data.sensor4 - 2*data.sensor5;
        // Release trajectory planning mutex
        pthread_mutex_unlock(&t);
        pthread_mutex_unlock(&s);
        pthread_mutex_unlock(&m);
        // Sleep for task period
        usleep(t_sleep);
    }
    return NULL;
}

int main() {
    // Threads for each task
    pthread_t m_t, s_t, t_t;
    
    // Set scheduling policy to SCHED_FIFO for real-time behavior
    struct sched_param param;
    // int policy = SCHED_RR;
    int policy = SCHED_FIFO;
    
    // Adjust priority as needed
    // Create motor control thread
    param.sched_priority = 3;
    pthread_create(&m_t, NULL, motor_task, NULL);
    pthread_setschedparam(m_t, policy, &param);

    // Create sensor data processing thread
    param.sched_priority = 2;
    pthread_create(&s_t, NULL, sensor_task, NULL);
    pthread_setschedparam(s_t, policy, &param);

    // Create trajectory planning thread
    param.sched_priority = 1;
    pthread_create(&t_t, NULL, trajectory_task, NULL);
    pthread_setschedparam(t_t, policy, &param);
    
    // Main thread acts as the real-time task scheduler
    
    // Wait for threads to finish (should never happen) and join them
    pthread_join(m_t,NULL);
    pthread_join(s_t,NULL);
    pthread_join(t_t,NULL);
    return 0;
}
