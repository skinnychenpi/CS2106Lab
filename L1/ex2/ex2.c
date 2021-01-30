/*************************************
* Lab 1 Exercise 1
* Name: Chen Yuheng
* Matric No: A0229929L
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);
void printList(node*);
void destroyList(node*);
// Helper Prototypes
node* addToHead(node* head, int newData);
node* insertAtSingle(node* head, int position, int newValue);


int main()
{
    node* myList = NULL;    //Empty List
    int position, copies, newValue;

    //Fill in code for input and processing
    while (scanf("%d%d%d",&position,&newValue,&copies) == 3){
        myList = insertAt(myList,position,copies,newValue);
    }

    //Output code coded for you
    printf("My List:\n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL;

    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
    //Fill in your code here
    while(copies >= 1){
        head = insertAtSingle(head,position,newValue);
        copies--;
    }
    return head;
}
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node* ptr = head;

    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head)
{
    //Fill in your code here
    //You can use the same implementation as in exercise 1
    node* tmp;
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
        tmp = NULL;
    }
    
}

node* addToHead(node* head, int newData)
{
    //Fill in your code here
    node* newNode;
    newNode = (node*) malloc(sizeof(node));
    newNode->data = newData;
    newNode->next = head;
    return newNode;    //change this
}

node* insertAtSingle(node*head, int position, int newValue){
    if (head == NULL || position == 0){
        return addToHead(head,newValue);
    }

    int beforeIndex = 0;
    node* cursor = head;

    while (cursor->next != NULL && beforeIndex != position - 1){      
        beforeIndex++;
        cursor = cursor->next;
    }

    cursor->next = addToHead(cursor->next,newValue);

    return head;    //change this
}