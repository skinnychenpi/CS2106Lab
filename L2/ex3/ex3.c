/*************************************
* Lab 2 Exercise 3
* Name: Chen Yuheng
* Student Id: A0229929L
* Lab Group: SOLO
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
Note2: Exercise 3 is a superset of exercise 2
Please copy over your code to ex3.c and continue
Submit ONLY ex3.c (even if you cannot finish ex3)
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()
#include <signal.h>


char** split( char* input, char* delimiter, int maxTokenNum, int* readTokenNum )
//Assumptions:
//  - the input line is a string (i.e. with NULL character at the end)
//  - the delimiter is a string of possible delimiters, each delimiter is single chracter
//Behavior:
//  - Split up to and include maxTokenNum from the input string
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - Must use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* tStart;   //start of token
    int i, strSize;      

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    //Look for next token as separated by the delimiter
    tStart = strtok(input, delimiter); 

    i = 0;
    while (i < maxTokenNum && tStart){

        strSize = strlen(tStart);

        //Allocate space for token string. +1 for null terminator
        tokenStrArr[i] = (char*) malloc(sizeof(char) * (strSize + 1) );

        strcpy(tokenStrArr[i], tStart);    

        i++;
        tStart = strtok(NULL, delimiter); 
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}
void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}
void doShowPath(char *path);
void doSetPath(char *path, char **cmdLineArgs);
void doExecution(char *filePath, char *fileArgs[],int *resultStatus);
void getExecutionFileName(char *filePath, char *path, char *fileName);
void getFileArgs(char *fileArgs[], char **cmdLineArgs, int *runInBg);
void doShowResult(int resultStatus);
void doExecutionInBg(char *filePath, char *fileArgs[], int *bgPIDList);
void doWaitPID(int *bgPIDList, char *userPIDChar, int *resultStatus);
void doPrintChild(int *bgPIDList);
void parentHandler(int signo);
void interruptChildHandler(int signo);
void ignoreHandler(int signo);

// Set a vairable called currentlyWaitingPID for intterrupt handler.
int currentlyWaitingPID = 0;

int main() {
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];

    int tokenNum;

    // Set default value = -1 and if it is less than 0, it means no child process has ran before.
    int resultStatus = -1;
    int bgPIDList[10] = {0,0,0,0,0,0,0,0,0,0};


    //read user input
    printf("YWIMC > ");
    
    // Check whether there is a interrupt signal received.
    if (signal(SIGINT, parentHandler) == SIG_ERR) {
        printf("Failed to register parent handler.\n");
        exit(-1);
    }
    
    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ) {
        
        // Check whether there is a interrupt signal received.
        if (signal(SIGINT, parentHandler) == SIG_ERR) {
            printf("Failed to register parent handler.\n");
            exit(-1);
        }
        
        char* cmdKey = cmdLineArgs[0];
        //Figure out which command the user want and implement below
        if (strcmp(cmdKey,"showpath") == 0) {
            doShowPath(path);
        } else if (strcmp(cmdKey,"setpath") == 0 && cmdLineArgs[2] == NULL) {
            doSetPath(path,cmdLineArgs);
        } else if (strcmp(cmdKey,"result") == 0 && cmdLineArgs[2] == NULL && resultStatus >= 0) {
            doShowResult(resultStatus);
        } else if (strcmp(cmdKey,"wait") == 0 && cmdLineArgs[2] == NULL){
            doWaitPID(bgPIDList,cmdLineArgs[1], &resultStatus);
        }  else if (strcmp(cmdKey,"pc") == 0 && cmdLineArgs[1] == NULL){
            doPrintChild(bgPIDList);
        } else {
            // Run executable in the shell
            char filePath[141] = "";
            getExecutionFileName(filePath, path, cmdLineArgs[0]);
            struct stat buf;
            int filePathExist = stat(filePath, &buf);
            
            if (filePathExist == 0) {
                char *fileArgs[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
                int runInBg = 0;

                getFileArgs(fileArgs,cmdLineArgs,&runInBg);
                if (runInBg) {
                    doExecutionInBg(filePath, fileArgs, bgPIDList);
                } else {
                    doExecution(filePath,fileArgs,&resultStatus);
                }
            } else {
                printf("'%s' not found.\n\n", filePath);
            }
        }

        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);

        printf("YWIMC > ");
        fgets(userInput, 120, stdin);
        cmdLineArgs = split( userInput, " \n", 7, &tokenNum );
    }
    

    printf("Goodbye!\n");

    //Clean up the token array as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);


    return 0;

}


void doShowPath(char *path) {
    printf("%s \n",path);
    printf("\n");
}

void doSetPath(char* path, char **cmdLineArgs) {
    int i = 0;
    char* targetPath = cmdLineArgs[1];
    while (targetPath[i] != '\0') {
        path[i] = targetPath[i];
        i++;
    }
    path[i] = '\0';
    printf("\n");
}

void doExecution(char *filePath, char *fileArgs[], int *resultStatus) {
    int result = fork();
    // For child process:
    if (result == 0) {
        execv(filePath, fileArgs);
        // If the exection is wrong, which means the child process can't exit through execv, then we force it to exit.
        _exit(1);
    } else {
        // For parent process:
        currentlyWaitingPID = result;
        signal(SIGINT, interruptChildHandler);      
        waitpid(result,resultStatus,0);
    }
    printf("\n");
}

void getExecutionFileName(char *filePath, char *path, char *fileName) {
    strncat(filePath, path, 20);
    strncat(filePath,"/",2);
    strncat(filePath, fileName, 121);
}

void getFileArgs(char *fileArgs[], char **cmdLineArgs, int *runInBg) {
    int i = 0;
    while (cmdLineArgs[i] != NULL) {
        if (strcmp(cmdLineArgs[i],"&") == 0) {
            *runInBg = 1;
            return;
        }
        fileArgs[i] = cmdLineArgs[i];
        i++;
    }
}

void doShowResult(int resultStatus) {
    int returnValue = WEXITSTATUS(resultStatus);
    printf("%d\n",returnValue);
    printf("\n");
}

void doExecutionInBg(char *filePath, char *fileArgs[], int *bgPIDList) {
    int result = fork();
    // For child process:
    if (result == 0) {
        // Set a new process group with the child process itself as the group leader so that it ignores the interrupt signal from parents.
        setpgid(getpid(),getpid());
        // Now execute the file.        
        execv(filePath, fileArgs);
        // If the exection is wrong, which means the child process can't exit through execv, then we force it to exit.
        _exit(1);
    } else {
        // For parent process:     
        printf("Child %d in background\n",result);
        
        // Add the PID into the PID array.
        int i = 0;
        while (bgPIDList[i] != 0) {
            i++;
        }
        bgPIDList[i] = result;

    }
    printf("\n");
}

void doWaitPID(int *bgPIDList, char *userPIDChar, int *resultStatus) {
    int userPID = atoi(userPIDChar);
    int PIDExist = 0;
    
    // Check Validness through bgPIDList.
    int i = 0;
    while (i < 10) {
        if (bgPIDList[i] == userPID) {
            PIDExist = 1;
            break;
        } 
        i++;
    }

    if (PIDExist) {
        currentlyWaitingPID = userPID;
        signal(SIGINT, interruptChildHandler); 
        waitpid(userPID,resultStatus,0);
        // Set the postion in the bgPIDList as 0, which means the process has been waited.
        bgPIDList[i] = 0;
    }
    else printf("%d not a valid child pid\n", userPID);
    printf("\n");

}

void doPrintChild(int *bgPIDList) {
    printf("Unwaited Child Processes: \n");
    for (int i = 0; i < 10; i++) {
        if (bgPIDList[i] != 0) {
            printf("%d\n",bgPIDList[i]);
        }
    }
    printf("\n");
}

void parentHandler(int signo)
{
    if (signo == SIGINT){
        printf("Nothing to kill.\n");
    }
}

void interruptChildHandler(int signo)
{
    if (signo == SIGINT){
        kill(currentlyWaitingPID,SIGINT);
    }
}

void ignoreHandler(int signo)
{
    if (signo == SIGINT){
        printf("I am ignored in the background!");
    }
}