// robot.c file
#include <stdlib.h> // C standard library header
#include <stdio.h> // Input/output library header
#include <errno.h> // Error code handling lib
#include <string.h> // String manager lib

int main(int argc, char *argv[]) {
    // argv[1] - path/avoidance
    
    // Integer pointers
    int *right_wheel_speed;
    int *left_wheel_speed;

    // Character pointer
    char *robot_name;
    int i;
    size_t n;

    // check mode
    if(strcmp(argv[1], "path") && strcmp(argv[1], "avoidance")){
        printf("Incorrent mode: %s\n", argv[1]);
        return 0;
    }

    // Dynamic memory allocation using malloc
    right_wheel_speed = (int *)malloc(1 * sizeof(int));
    left_wheel_speed = (int *)malloc(1 * sizeof(int));

    if(!strcmp(argv[1], "path")){
        for(int i=0; i < 100; i++){
            *left_wheel_speed = i;
            *right_wheel_speed = i;
        }
        for(int i=100; i > 0; i-=2){
            *left_wheel_speed = i;
            *right_wheel_speed = i;
        }
    }else{ // avoidance
        *left_wheel_speed = -10;
        *right_wheel_speed = 10;
        for(int i=0; i < 10; i++);
        *left_wheel_speed = 10;
        *right_wheel_speed = -10;
        for(int i=0; i < 10; i++);
    }

    free(right_wheel_speed);
    free(left_wheel_speed);
    return 0;
}
