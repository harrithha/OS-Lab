#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <unistd.h>

// Program to print square of a given number

int main(int argc, char *argv[])
{
    int num = atoi(argv[argc-1]);
    int pid = getpid();
    int result = num*num;

    printf("Square: Current process id: %d, Current result: %d\n", pid, result);
    if(argc>2)
    {
        sprintf(argv[argc-1], "%d", result);
        execvp(argv[1], argv + 1);
    }
    
    return 0;
}