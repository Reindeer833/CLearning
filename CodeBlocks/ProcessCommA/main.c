#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


void doBranching(void);
void childRunning(pid_t parrent);
int file_pipes[2];
char* cPtr;
bool runAlong = true;

void sigint_handler();

void sigint_handler()
{
    printf("\n%s got a message\n",cPtr);
    runAlong = false;
}

void psigint_handler()
{
    printf("\n%s got a message\n",cPtr);
}

int main()
{
    printf("Hello world!\n\n");

    doBranching();

    return 0;
}


void doBranching(void)
{
    pid_t parentPid = getpid();
    pid_t aPid;
    int ix = 0;
    pipe(file_pipes);


    aPid = fork();

    if(aPid == 0)//child
    {
      childRunning(parentPid);
    }

    else
    {
        char aBff[64];
        cPtr = "Parent";
        signal(SIGINT,psigint_handler);
        while(ix++< 4)
        {
            read(file_pipes[0],aBff,64);
            printf("\n---> %s\n",aBff);
            sleep(1);
        }
        printf("\n%s sending signal.\n",cPtr);
        kill(aPid,SIGINT);
    }

}

void childRunning(pid_t parrent)
{
    signal(SIGINT,sigint_handler);
    cPtr = "Child";
    kill(parrent,SIGINT);

    while(runAlong)
    {
        printf("\nRunning %s\n",cPtr);
        write(file_pipes[1],"Hello father!",strlen("Hello father!")+1);
        sleep(1);
    }
     printf("\nFinished running %s\n",cPtr);
}
