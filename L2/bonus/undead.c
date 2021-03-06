#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void myOwnHandler(int signo)
{
    if (signo == SIGINT){
        printf("You cannot kill me! Bwahahaha!\n");
    }
}

int main()
{
    if (signal(SIGINT, myOwnHandler) == SIG_ERR){
        printf("Failed to register handler\n");
        exit(-1);
    }

    //Infinite loop......
    while(1);
    

    return 0;
}
