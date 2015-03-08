#include <stdio.h>      // printf
#include <stdlib.h>     // malloc
#include "LinkedList.h"

void main(void) {

    struct LinkedList *list = malloc(sizeof(struct LinkedList));

    // Quick initialization of node
    struct Node node = {8, NULL};
    printf("node.val = %d \n", node.val);

    // Insert a node into list
    list->header = &node;
    list->tail = &node;
    printf("list's header node val = %d \n", list->header->val);
    printf("list's tail node val = %d \n", list->tail->val);
    
    // Check if next is null (it should be)
    if (list->header->next == NULL) {printf("next is null\n");}
    else {printf("next is not null\n");}

    // Push a bunch of nodes onto list
    int i;
    for (i = 20; i < 30; i++) {
        // Making anotherNode as a pointer and using malloc ensures that new memory is allocated in every iteration of the for loop.
        struct Node *anotherNode = malloc(sizeof(struct Node));
        // Initialize node members
        anotherNode->val = i;
        // anotherNode now points to front of list
        anotherNode->next = list->header;
        // header points to anotherNode (front of list)
        list->header = anotherNode;
    }

    // debug
    printf("list's header node val = %d \n", list->header->val);
    printf("list's next node val = %d \n", list->header->next->val);
    printf("list's next next node val = %d \n", list->header->next->next->val);
    printf("list's tail node val = %d \n", list->tail->val);
    // exit(0);
    // end debug

    // Loop through list and print out node values
    struct Node *ptr;
    printf("Printing out list\n");
    for (ptr = list->header; ptr != NULL; ptr = ptr->next) {
        printf("%d \n", ptr->val);
    }

    // Append a bunch of nodes onto list
    for (i = -1; i > -9; i--) {
        struct Node *anotherNode = malloc(sizeof(struct Node));
        anotherNode->val = i;
        anotherNode->next = NULL;
        list->tail->next = anotherNode;
        list->tail = anotherNode;
    }

    // Print list
    printf("Printing after appends\n");
    for (ptr = list->header; ptr != NULL; ptr = ptr->next) {
        printf("%d \n", ptr->val);
    }
}
