/*************************************
* Lab 2 Exercise 1
* Name: Chen Yuheng
* Student Id: A0229929L
* Lab Group: Solo
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

int main()
{
    int nChild; 

    //Read the number of child
    scanf("%d", &nChild);

    //Spawn child processes
    int childPidList[nChild];
    

    for (int i = 0; i < nChild; i++) {
        int result = fork();
        // For child process:
        if (result == 0) {
            int childPid = getpid();
            printf("Child %i [%i] : Hello!\n", i + 1, childPid);
            _exit(0);
        }
        // For parent process:
        else childPidList[i] = result;
    }

    //Wait on child processes in order
    for (int i = 0; i < nChild; i++) {
        int childPid = childPidList[i];
        waitpid(childPid, NULL, 0);
        printf("Parent: Child %i [%i] done.\n", i + 1, childPid);
    }
    printf("Parent: Exiting.\n");

    return 0;
}
