/*************************************
* Lab 1 Exercise 3
* Name     :
* Matric No:
* Lab Group:
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int lumos(int x, int y);
int alohomora(int x, int y);
int expelliarmus(int x, int y);
int sonorus(int x, int y);


int main()
{
    int a, b, optype, res;

    //The code below is to show the usage of a function pointer only
    //You can remove/comment them off 

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result.
    
    char spellNames[4][15] = { "lumos", "alohomora", "expelliarmus", "sonorus"};
    arithFuncPtr spells[4] = {lumos, alohomora, expelliarmus, sonorus};
    while (scanf("%i%i%i",&optype,&a,&b) == 3){
        res = (*(spells[optype - 1]))(a,b);
        printf("%s ",spellNames[optype - 1]);
        printf("%i\n",res);
    }

    //ptr points to the function add


    // printf("%s\n", spellNames[0]);    

    // printf("%i\n", res);
   
    return 0;
}


int lumos(int x, int y){
    return ((x + y) * (y - x + 1) / 2);
}

int alohomora(int x, int y){
    if(x % y == 0){
        return y;
    }
    return alohomora(y, x % y);
}

int expelliarmus(int x, int y){
    if (x % y != 0){
        return x;
    }
    return expelliarmus(x / y, y);
}

int sonorus(int x, int y){
    int result = 1;
    while (y > 0){
        result *= x;
        y--;
    }
    return result;
}