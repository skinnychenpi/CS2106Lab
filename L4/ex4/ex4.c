/*************************************
* Lab 4 Exercise 3
* Name: Chen Yuheng
* Student Id: A0229929L
* Lab Group: SOLO
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmalloc.h"

/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;


/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

unsigned int log2Ceiling( unsigned int N )
/**********************************************************
 * Find the smallest S, such that 2^S >= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 < N){
        pOf2 <<= 1;
        s++;
    }

    return s;
}


unsigned int log2Floor( unsigned int N )
/**********************************************************
 * Find the largest S, such that 2^S <= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 <= N){
        pOf2 <<= 1;
        s++;
    }

    return s-1;
}

unsigned int buddyOf( unsigned int addr, unsigned int lvl )
/**********************************************************
 * Return the buddy address of address (addr) at level (lvl)
 *********************************************************/
{
    unsigned int mask = 0xFFFFFFFF << lvl;
    unsigned int buddyBit = 0x0001 << lvl;

    return (addr & mask) ^ buddyBit;
}

partInfo* buildPartitionInfo(unsigned int offset)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
	piPtr->nextPart = NULL;

    //Buddy system's partition size is implicit
	//piPtr->size = size;

    //All available partition in buddy system is implicitly free
	//piPtr->status = FREE;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
	partInfo* current;
    int count = 1;
	
	for ( current = piPtr; current != NULL; 
		current = current->nextPart){
        if (count % 8 == 0){
            printf("\t");
        }
		printf("[+%5d] ", current->offset);
        count++;
        if (count % 8 == 0){
            printf("\n");
        }
	}
    printf("\n");
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
    int i;

	printf("\nHeap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);

    for (i = hmi.maxIdx; i >=0; i--){
        printf("A[%d]: ", i);
        printPartitionList(hmi.A[i] );
    }

}

void printHeap()
/**********************************************************
 * Print the content of the entire Heap 
 *********************************************************/
{
    //Included as last debugging mechanism.
    //Print the entire heap regions as integer values.

    int* array;
    int size, i;
    
    size = hmi.totalSize / sizeof(int);
    array = (int*)hmi.base;

    for ( i = 0; i < size; i++){
        if (i % 4 == 0){
            printf("[+%5d] |", i);
        }
        printf("%8d",array[i]);
        if ((i+1) % 4 == 0){
            printf("\n");
        }
    }
}

void printHeapStatistic()
/**********************************************************
 * Print Heap Usage Statistics
 *********************************************************/
{
    //TODO: Task 4. Calculate and report the various statistics

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

    printf("Total Space: %d bytes\n", hmi.totalSize);

   //Remember to preserve the message format!

    int totalFreePartition = 0;
    int totalFreeSize = 0;
    for (int lvl = 1; lvl <= hmi.maxIdx; lvl++) {
        int levelSize = 1;
        levelSize <<= lvl;
        partInfo* levelCursor = hmi.A[lvl];
        while (levelCursor != NULL) {
            totalFreePartition++;
            totalFreeSize += levelSize;
            levelCursor = levelCursor->nextPart;
        }
    }
    
    printf("Total Free Partitions: %d\n", totalFreePartition);
    printf("Total Free Size: %d bytes\n", totalFreeSize);

    printf("Total Internal Fragmentation: %d bytes\n", hmi.internalFragTotal);
}

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function adds a new free partition with "offset" at hmi.A[lvl]
 *    If buddy is found, recursively (or repeatedly) perform merging and insert
 *      at higher level
 *********************************************************/
{   
    partInfo* levelHead = hmi.A[lvl];
    partInfo* findBuddyCursor;
    partInfo* findBuddyPrevCursor;
    int buddyOffset;
    if (lvl < hmi.maxIdx) {
        findBuddyCursor = levelHead;
        findBuddyPrevCursor = NULL;
        // Find Buddy
        buddyOffset = buddyOf(offset, lvl);
        // Do linear search to find the buddy.
        while (findBuddyCursor != NULL) {
        if (findBuddyCursor->offset == buddyOffset) {
            break;
        }
        findBuddyPrevCursor = findBuddyCursor;
        findBuddyCursor = findBuddyCursor->nextPart;
    }
    } else {
        findBuddyCursor = NULL;
    }
    
    // If we find a buddy:
    if (findBuddyCursor != NULL) {
        int nextLevelOffset = (buddyOffset < offset) ? buddyOffset : offset;
        // remove the buddy
        if (findBuddyPrevCursor == NULL) {
            levelHead = findBuddyCursor->nextPart;
            findBuddyCursor->nextPart = NULL;
            hmi.A[lvl] = levelHead;
        } else {
            findBuddyPrevCursor->nextPart = findBuddyCursor->nextPart;
            findBuddyCursor->nextPart = NULL;
        }
        addPartitionAtLevel(lvl + 1, nextLevelOffset);
    } else {
        // Else just do insertion
        partInfo* cursor = levelHead;
        partInfo* prevCursor = NULL;

        partInfo* toAdd = malloc(sizeof(partInfo));
        toAdd->offset = offset;
        toAdd->nextPart = NULL;

        if (levelHead == NULL) {
            levelHead = toAdd;
        }
        while (cursor != NULL && cursor->offset < offset) {
            prevCursor = cursor;
            cursor = cursor->nextPart;
        }
        // If the new one becomes the head.
        if (prevCursor == NULL) {
            levelHead = toAdd;
            toAdd->nextPart = cursor;
        } else {
            prevCursor->nextPart = toAdd;
            toAdd->nextPart = cursor;
        }
        hmi.A[lvl] = levelHead;
    }

}

partInfo* removePartitionAtLevel(unsigned int lvl)
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function remove a free partition at hmi.A[lvl]
 *    Perform the "upstream" search if this lvl is empty AND perform
 *      the repeated split from higher level back to this level.
 * 
 * Return NULL if cannot find such partition (i.e. cannot sastify request)
 * Return the Partition Structure if found.
 *********************************************************/
{
    partInfo* levelPart = hmi.A[lvl];
    if (levelPart == NULL) {
        return NULL;
    } else {
        partInfo* newHead = levelPart->nextPart;
        hmi.A[lvl] = newHead;
        levelPart->nextPart = NULL;
        return levelPart;
    }
}

int setupHeap(int initialSize, int minPartSize, int maxPartSize)
/**********************************************************
 * Setup a heap with "initialSize" bytes
 *********************************************************/
{
	void* base;

	base = sbrk(0);
	if(	sbrk(initialSize) == (void*)-1){
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

    hmi.base = base;
	
    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    int maxIdx = log2Floor(maxPartSize);
    int minIdx = log2Floor(minPartSize);
    hmi.A = (partInfo**) malloc((maxIdx + 1) * sizeof(partInfo*));
    hmi.maxIdx = maxIdx; 
    hmi.minIdx = minIdx;
    for (int i = minIdx; i < maxIdx + 1; i++) {
        hmi.A[i] = NULL;
    }
    hmi.internalFragTotal = 0;
    hmi.totalSize = initialSize - (initialSize & (minPartSize - 1));
    

    int currentAddress = 0;
    // Deal with bit that is larger or equal to maxIdx
    int unlimitedMaxIdx = log2Floor(initialSize);
    if (unlimitedMaxIdx > maxIdx) {
        int numOfPartAtLevelMax = initialSize >> maxIdx;
        int levelSize = maxPartSize;
        partInfo* maxLevelHead = NULL;
        partInfo* prevMaxLevelPart = NULL;
        // Form the linked list at hmi.A[maxIdx]
        while (numOfPartAtLevelMax > 0) {
            // get new partInfo
            partInfo* initialPart = malloc(sizeof(partInfo));
            initialPart->offset = currentAddress;
            initialPart->nextPart = NULL;
            if (maxLevelHead == NULL) {
                maxLevelHead = initialPart;
            } else {
                prevMaxLevelPart->nextPart = initialPart;
            }
            prevMaxLevelPart = initialPart;

            currentAddress += levelSize;
            numOfPartAtLevelMax--;
        }
        hmi.A[maxIdx] = maxLevelHead;
    }
    // Deal with rest of the bits
    for (int i = maxIdx - 1; i >= minIdx; i--) {
        int tmp = initialSize >> i;
        int bit = tmp & 1;
        if (bit == 1) {
            partInfo* initialPart = malloc(sizeof(partInfo));
            initialPart->offset = currentAddress;
            initialPart->nextPart = NULL;
            hmi.A[i] = initialPart;
            int levelSize = 1;
            levelSize <<= i;
            currentAddress += levelSize;
        }
    }

    return 1;
}


void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory addres of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Task 2. Implement the allocation using buddy allocator
    int S = log2Ceiling(size);
    if (S < hmi.minIdx) {
        S = hmi.minIdx;
    }
    if (S > hmi.maxIdx) {
        return NULL;
    }
    int levelSSize = 1;
    levelSSize <<= S;
    int internalFrag = levelSSize - size;
    // printf("IN MYMALLOC, THE INTERNAL FRAG IS: %d\n", internalFrag);

    partInfo *levelSPart = removePartitionAtLevel(S);

    if (levelSPart != NULL) {
        hmi.internalFragTotal += internalFrag;
        return (void*)hmi.base + levelSPart->offset;
    } else {
        int R = S;
        partInfo *levelRPart = removePartitionAtLevel(R);
        // printf("BEFORE THE CHECK THE R VALUE IS %d\n",R);
        while (levelRPart == NULL) {
            R++;
            if (R > hmi.maxIdx && levelRPart == NULL) {
                return NULL;
            }
            levelRPart = removePartitionAtLevel(R);
        }
        int K = R - 1; 
        while (K >= S) {
            // printf("I am here! AND THE K VALUE IS %d\n",K);
            partInfo *newPart = malloc(sizeof(partInfo));
            newPart->nextPart = NULL;
            int size = 1;
            size <<= K;
            newPart->offset = levelRPart->offset + size;
            hmi.A[K] = newPart;
            K--;
        }
        hmi.internalFragTotal += internalFrag;
        return (void*)hmi.base + levelRPart->offset;
    }

}

void myfree(void* address, int size)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator
    int levelToSearch = log2Ceiling(size);
    
    int levelSize = 1;
    levelSize <<= levelToSearch;
    int internalFrag = levelSize - size;
    
    int offset = address - hmi.base;
    addPartitionAtLevel(levelToSearch, offset);
    hmi.internalFragTotal -= internalFrag;
    

}
