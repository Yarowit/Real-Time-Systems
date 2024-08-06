// robot.c file
#include <stdlib.h> // C standard library header
#include <stdio.h> // Input/output library header
#include <errno.h> // Error code handling lib
#include <string.h> // String manager lib

int main(int argc, char *argv[]) {
    // argc - number of input arguments + 1
    // argv[] - array of input arguments
    // argv[0] = PATH/program-name
    // argv[1] = argument 1
    // argv[i] = argument i
    // argv[argc] = last entered argument
    // argv[>argc] = local data (machine name, etc.)
    
    // Integer pointers
    int *right_wheel_speed;
    int *left_wheel_speed;

    // Character pointer
    char *robot_name;
    int i;
    size_t n;

    // Dynamic memory allocation using malloc
    right_wheel_speed = (int *)malloc(1 * sizeof(int));
    left_wheel_speed = (int *)malloc(1 * sizeof(int));
    robot_name = (char *)malloc(15 * sizeof(char));

    // Assignment of memory pointer content
    *right_wheel_speed = 29;
    printf("(1) The right wheel speed is %d\n", *right_wheel_speed);
    *left_wheel_speed = 102;
    printf("(1) The left wheel speed is %d\n", *left_wheel_speed);
    left_wheel_speed = right_wheel_speed;
    *right_wheel_speed = 209;
    printf("(2) The right wheel speed is %d\n", *right_wheel_speed);
    printf("(2) The left wheel speed is %d\n", *left_wheel_speed);
    
    // Character array assignment
    strcpy(robot_name, "This is the name of your robot");
    n = strlen(robot_name);
    printf("(3) Robot name[%d] = %s\n", (int)n, robot_name);
    printf("(4) The name starts with %c\n",*robot_name);
    robot_name = robot_name + 1;
    printf("(5) The second letter of the name is %c\n", *robot_name);

    // Free dynamic memory using free command
    free(right_wheel_speed);
    free(left_wheel_speed);
    free(robot_name);
    return 0;
}
