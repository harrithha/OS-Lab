#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <unistd.h>
#include<string.h>

int main()
{
    char message[] = "Hello World";
    //setbuf(stdout, NULL);
    for (int i = 0; i < strlen(message); i++)
    {
        int childPid = fork();
        if(childPid == 0)
        {
            printf("%c %d\n", message[i], getpid());
            sleep((rand()%4)+1);
        }
        else
        {
            wait(NULL);
            break;
        }
    }
    return 0;
}