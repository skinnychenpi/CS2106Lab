/*************************************
* Lab 2 Exercise 2
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
void doExecution(char *filePath);
void getExecutionFileName(char *filePath, char *path, char *fileName);


int main() {
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];

    int tokenNum;

    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ) {
        char* cmdKey = cmdLineArgs[0];
        //Figure out which command the user want and implement below
        if (strcmp(cmdKey,"showpath") == 0) {
            doShowPath(path);
        } else if (strcmp(cmdKey,"setpath") == 0 && cmdLineArgs[2]  == NULL) {
            doSetPath(path,cmdLineArgs);
        } else {
            char filePath[141] = "";
            getExecutionFileName(filePath, path, cmdLineArgs[0]);
            struct stat buf;
            int status = stat(filePath, &buf);
            
            if (status == 0) {
                doExecution(filePath);
            } else {
                printf("'%s' not found.\n", filePath);
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
}

void doSetPath(char* path, char **cmdLineArgs) {
    int i = 0;
    char* targetPath = cmdLineArgs[1];
    while (targetPath[i] != '\0') {
        path[i] = targetPath[i];
        i++;
    }
    path[i] = '\0';
}

void doExecution(char *filePath) {
    int result = fork();
    // For child process:
    if (result == 0) {
        execl(filePath, filePath,(char*) NULL);
    } else {
        // For parent process:
        waitpid(result,NULL,0);
    }
}

void getExecutionFileName(char *filePath, char *path, char *fileName) {
    strncat(filePath, path, 20);
    strncat(filePath,"/",2);
    strncat(filePath, fileName, 121);
}