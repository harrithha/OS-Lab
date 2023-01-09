#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <unistd.h>

void printChar(char msg[], int c, int length)
{
    if(c < length)
    {
        int pid = fork();
        if(pid>0)
        {
            printf("%c %d\n", msg[c], pid);
            sleep((rand()%4)+1);
            printChar(msg, c+1, length);
        }
    }
}

int main()
{
    char message[] = "Hello World";
    setbuf(stdout, NULL);
    printChar(message, 0, 11);
    return 0;
}