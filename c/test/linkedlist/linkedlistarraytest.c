#include <stdio.h>      // printf
#include <stdlib.h>     // malloc
#include <string.h>     // memcpy
#include "LinkedListArray.h"

void main(void) {
    
    printf(">>> Linked List Array Test <<<\n\n");
    
    printf("Created a null linked list: ");
    struct LinkedListArray *list = NULL;
    llaPrint(list);
    
    printf("Created an empty linked list: ");
    list = llaInit();
    llaPrint(list);
    
    printf("Created a null node. Appending null node to list: ");
    struct NodeArray *nullNode = NULL;
    llaAppend(list, nullNode);
    llaPrint(list);
    printf("Pushing a null node onto list: ");
    llaPush(list, nullNode);
    llaPrint(list);
    
    printf("Creating a node with empty array... ");
    struct NodeArray *node = nodeArrayInit();
    nodeArrayPrint(node);
    printf("\n");
    
    printf("Appending node to list: ");
    llaAppend(list, node);
    llaPrint(list);

    printf("Pushed a bunch of nodes onto list: ");
    
    int i;
    
    for (i = 6; i < 10; i++) {
        struct NodeArray *aNode = nodeArrayInit();
        memcpy(aNode->val, (int [4]){i, i+1, i+2, i+3}, 4*sizeof(int));
        //aNode->val = {i, i+1, i+2, i+3};
        llaPush(list, aNode);
    }
    
    llaPrint(list);
    
    printf("Popped node from front of list. ");
    nodeArrayPrint(llaPop(list));
    llaPrint(list);
    
    printf("Popped node from back of list. ");
    nodeArrayPrint(llaPopBack(list));
    llaPrint(list);
    
    printf("Popped all nodes from list, from both front and back: ");
    for (i = 0; i < 100000; i++) {
        llaPopBack(list);
        llaPop(list);
        llaPopBack(list);
    }
    
    llaPrint(list);
    
    printf("Popped another node from front of empty list. It should be null. Is it null? ");
    if (llaPop(list) == NULL) {printf("Yes.\n");}
    else                     {printf("***No!***\n");}
    
    printf("Nodes pushed on and appended to list: ");
    for (i = -1; i > -6; i--) {
        struct NodeArray *aNode = nodeArrayInit();
        //aNode->val = {i, i-1, i-2, i-3};
        memcpy(aNode->val, (int [4]){i, i-1, i-2, i-3}, 4*sizeof(int));
        llaAppend(list, aNode);
    }
    for (i = 0; i < 6; i++) {
        struct NodeArray *aNode = nodeArrayInit();
        //aNode->val = {2*i, 3*i, 4*i, 5*i};
        memcpy(aNode->val, (int [4]){2*i, 3*i, 4*i, 5*i}, 4*sizeof(int));
        llaPush(list, aNode);
    }
    
    llaPrint(list);
}
