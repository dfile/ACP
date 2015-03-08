#include <stdio.h>      // printf
#include <stdlib.h>     // malloc
#include "LinkedList.h"

void main(void) {
    
    printf(">>> Linked List Test <<<\n\n");
    
    printf("Created a null linked list: ");
    struct LinkedList *list = NULL;
    llPrint(list);
    
    printf("Created an empty linked list: ");
    list = llInit();
    llPrint(list);
    
    printf("Created a null node. Appending null node to list: ");
    struct Node *nullNode = NULL;
    llAppend(list, nullNode);
    llPrint(list);
    printf("Pushing a null node onto list: ");
    llPush(list, nullNode);
    llPrint(list);
    
    printf("Creating a node with value of 5... ");
    struct Node *node = nodeInit();
    
    node->val = 5;
    
    printf("Created node. node->val = %d \n", node->val);
    printf("Appending node to list: ");
    llAppend(list, node);
    llPrint(list);

    printf("Pushed a bunch of nodes onto list: ");
    
    int i;
    
    //printf("Starting for loop\n");
    for (i = 6; i < 10000; i++) {
        //printf("--Beginning of for loop\n");
        struct Node *aNode = nodeInit();
        aNode->val = i;
        llPush(list, aNode);
        //printf("%d th iteration. ", i);
        //llPrint(list);
    }
    
    llPrint(list);
    
    printf("Popped node from front of list. ");
    printf("Node val = %d, ", llPop(list)->val);
    llPrint(list);
    
    printf("Popped node from back of list. ");
    printf("Node val = %d, ", llPopBack(list)->val);
    llPrint(list);
    
    printf("Popped all nodes from list, from both front and back: ");
    for (i = 0; i < 100000; i++) {
        llPopBack(list);
        llPop(list);
        llPopBack(list);
    }
    
    llPrint(list);
    
    printf("Popped another node from front of empty list. It should be null. Is it null? ");
    if (llPop(list) == NULL) {printf("Yes.\n");}
    else                     {printf("***No!***\n");}
    
    printf("Nodes pushed on and appended to list: ");
    for (i = -1; i > -6; i--) {
        struct Node *aNode = nodeInit();
        aNode->val = i;
        llAppend(list, aNode);
    }
    for (i = 0; i < 6; i++) {
        struct Node *aNode = nodeInit();
        aNode->val = i;
        llPush(list, aNode);
    }
    
    llPrint(list);
}