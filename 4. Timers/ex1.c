#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CLOCK_MONOTONIC 1
#define halfSec 500000000

// Structure to represent an internal process
struct InternalProcess {
    float value; // Current value of the process
    int lower_limit; // Lower limit of the process
    int upper_limit; // Upper limit of the process
    float variation_rate; // Variation rate of the process
    int ideal_value; // Ideal value of the process
    float restoreRate;
} Sleep = {0,0,100,0.2,0,-0.5},Energy = {100,0,100,-0.4,100,1.5},Entertainment = {100,0,100,-0.1,100,3.0};


// returns 1 on fail
int step(struct InternalProcess *proc){
    proc->value += proc->variation_rate;
    // printf("%f\n",proc->value);
    if(proc->value > proc->upper_limit || proc->value < proc->lower_limit)
        return 1;
}

void chooseAction(char* message){
    float sleepDeficit, energyDeficit, entertainmentDeficit;
    sleepDeficit = abs(Sleep.ideal_value - Sleep.value);
    energyDeficit = abs(Energy.ideal_value - Energy.value);
    entertainmentDeficit = abs(Entertainment.ideal_value - Entertainment.value);
    // printf("%f %f %f\n",sleepDeficit,energyDeficit,entertainmentDeficit);  
    
    if(sleepDeficit > energyDeficit && sleepDeficit > entertainmentDeficit){
        printf("Action chosen: Sleep\n");
        Sleep.value += Sleep.restoreRate - Sleep.variation_rate;
        if(Sleep.value < Sleep.lower_limit) Sleep.value = Sleep.lower_limit;
    }else if(energyDeficit > entertainmentDeficit){
        printf("Action chosen: Rest\n");
        Energy.value += Energy.restoreRate - Energy.variation_rate;
        if(Energy.value > Energy.upper_limit) Energy.value = Energy.upper_limit;
    }else{
        printf("Action chosen: Play\n");
        Entertainment.value += Entertainment.restoreRate - Entertainment.variation_rate;
        if(Entertainment.value > Entertainment.upper_limit) Entertainment.value = Entertainment.upper_limit;
    }
}

int main(){
    struct timespec goalTime, currentTime;
    char* message = "Nothing";
    clock_gettime(CLOCK_MONOTONIC, &goalTime);
    
    while(1){
        clock_gettime(CLOCK_MONOTONIC, &currentTime);

        // if given time has passed (half a second)
        if((currentTime.tv_sec == goalTime.tv_sec && currentTime.tv_nsec > goalTime.tv_nsec) ||
            currentTime.tv_sec > goalTime.tv_sec ){
            printf("----------------------------------------\n");
            printf("Half second passed\n");
            printf("CT: %ld %ld\n",currentTime.tv_sec,currentTime.tv_nsec);
            printf("GT: %ld %ld\n",goalTime.tv_sec,goalTime.tv_nsec);
            

            // step the simulation
            if(step(&Sleep)){
                printf("Sleep failed\n");
                break;
            }
            if(step(&Energy)){
                printf("Sleep failed\n");
                break;
            }
            if(step(&Entertainment)){
                printf("Sleep failed\n");
                break;
            }

            chooseAction(message);
            printf("State:\n");
            printf("    Sleep : %f\n",Sleep.value);
            printf("    Energy: %f\n",Energy.value);
            printf("    Entertainment: %f\n",Entertainment.value);


            if(goalTime.tv_nsec > halfSec){
                goalTime.tv_sec += 1;
                goalTime.tv_nsec -= halfSec;
            }else{
                goalTime.tv_nsec += halfSec;
            }
        }
    }
}
