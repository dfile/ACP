#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

// Create and return an empty linked list
struct LinkedList* llInit(void) {
    struct LinkedList *list = malloc(sizeof(struct LinkedList));
    list->header = NULL;
    list->tail = NULL;
    return list;
}

// Create and return an empty node
struct Node* nodeInit(void) {
    struct Node *node = malloc(sizeof(struct Node));
    node->val=0;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Push node on front of list
void llPush(struct LinkedList *ll, struct Node *node) {
    
    // Make sure list and node are not null
    if (ll != NULL && node != NULL) {
        
        node->next = NULL;
        node->prev = NULL;

        if (ll->header == NULL)       // list is empty
        {
            ll->header = node;
            ll->tail = node;
        }
        else                          // list is not empty
        {
            node->next = ll->header;  // node points to front of list
            ll->header->prev = node;  // former front of list points to node
            ll->header = node;        // header points to node
        }
    }
}

// Append node to back of list
void llAppend(struct LinkedList *ll, struct Node *node) {
    
    // Make sure list and node are not null
    if (ll != NULL && node != NULL) {
        
        node->next = NULL;
        node->prev = NULL;
        
        if (ll->header == NULL)       // list is empty
        {
            ll->header = node;
            ll->tail = node;
        }
        else                          // list is not empty
        {
            node->next = NULL;        // ensure last node is last
            node->prev = ll->tail;    // node's prev node is tail
            ll->tail->next = node;
            ll->tail = node;
        }
    }
}

// Remove and return node from front of list
struct Node* llPop(struct LinkedList *ll) {
    
    // Make sure list is not null
    if (ll != NULL) {
        
        if (ll->header != NULL)  // ll is not empty
        {
            struct Node *nodeToPop = ll->header;
            ll->header = ll->header->next;
            
            if (ll->header == NULL)    // nodeToPop was only element in ll
            {
                ll->tail = NULL;
            }
            
            return nodeToPop;
        }
            
    }
    
    return NULL;        // catch-all return null
}

// Remove and return node from back of list
struct Node* llPopBack(struct LinkedList *ll) {
    
    // Make sure list is not null
    if (ll != NULL) {
        
        if (ll->tail != NULL)  // ll is not empty
        {
            struct Node *nodeToPop = ll->tail;
            ll->tail = ll->tail->prev;
            
            if (ll->tail == NULL)    // nodeToPop was only element in ll
            {
                ll->header = NULL;
            }
            else                    // safe to reference next member of tail
            {
                ll->tail->next = NULL;
            }
            
            return nodeToPop;
        }
            
    }
    
    return NULL;        // catch-all return null
}

// Print a linked list using printf
void llPrint(struct LinkedList *ll) {
    
    if (ll == NULL)
    {
        printf("\nLinked list is NULL.\n");
        return;
    }
    
    struct Node *ptr;
    printf("\n[ ");
    for (ptr = ll->header; ptr != NULL; ptr = ptr->next) {
        printf("(%d ", ptr->val);
        if (ptr->prev == NULL) { printf("N"); }
        else { printf("S"); }
        if (ptr->next == NULL) { printf("N"); }
        else { printf("S)"); }
    }
    printf("NULL ]\n");
}
