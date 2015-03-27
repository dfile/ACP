#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedListArray.h"

// Create and return an empty linked list
struct LinkedListArray* llaInit(void) {
    struct LinkedListArray *list = malloc(sizeof(struct LinkedListArray));
    list->header = NULL;
    list->tail = NULL;
    list->len = 0;
    return list;
}

// Create and return an empty node
// Array is initialized with null elements.
struct NodeArray* nodeArrayInit(void) {
    struct NodeArray *node = malloc(sizeof(struct NodeArray)); 
    int i;
    for (i = 0; i < (sizeof(node->val) / sizeof(int)); i++)
    {
        node->val[i]=0;
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Create and return a node initialized with given array
struct NodeArray* nodeArrayInitWithArray(int arr[4]) {
    struct NodeArray *node = malloc(sizeof(struct NodeArray)); 
    memcpy(node->val, arr, 4*sizeof(int));
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Push node on front of list
void llaPush(struct LinkedListArray *lla, struct NodeArray *node) {
    
    // Make sure list and node are not null
    if (lla != NULL && node != NULL) {
        
        node->next = NULL;
        node->prev = NULL;
        lla->len = lla->len + 1;

        if (lla->header == NULL)       // list is empty
        {
            lla->header = node;
            lla->tail = node;
        }
        else                          // list is not empty
        {
            node->next = lla->header;  // node points to front of list
            lla->header->prev = node;  // former front of list points to node
            lla->header = node;        // header points to node
        }
    }
}

// Append node to back of list
void llaAppend(struct LinkedListArray *lla, struct NodeArray *node) {
    
    // Make sure list and node are not nulla
    if (lla != NULL && node != NULL) {
        
        node->next = NULL;
        node->prev = NULL;
        lla->len = lla->len + 1;
        
        if (lla->header == NULL)       // list is empty
        {
            lla->header = node;
            lla->tail = node;
        }
        else                          // list is not empty
        {
            node->next = NULL;        // ensure last node is last
            node->prev = lla->tail;    // node's prev node is tail
            lla->tail->next = node;
            lla->tail = node;
        }
    }
}

// Remove and return node from front of list
struct NodeArray* llaPop(struct LinkedListArray *lla) {
    
    // Make sure list is not nulla
    if (lla != NULL) {
        
        if (lla->header != NULL)  // lla is not empty
        {
            lla->len = lla->len - 1;
            // struct NodeArray *nodeToPop = nodeArrayInitWithArray(lla->header->val);
            struct NodeArray *nodeToPop = lla->header;
            lla->header = lla->header->next;
            
            if (lla->header == NULL)    // nodeToPop was only element in lla
            {
                lla->tail = NULL;
            }
            
            return nodeToPop;
        }
            
    }
    
    return NULL;        // catch-all return null
}

// Remove and return node from back of list
struct NodeArray* llaPopBack(struct LinkedListArray *lla) {
    
    // Make sure list is not nulla
    if (lla != NULL) {
        
        if (lla->tail != NULL)  // lla is not empty
        {
            lla->len = lla->len - 1;
            struct NodeArray *nodeToPop = nodeArrayInitWithArray(lla->tail->val);
            lla->tail = lla->tail->prev;
            
            if (lla->tail == NULL)    // nodeToPop was only element in lla
            {
                lla->header = NULL;
            }
            else                    // safe to reference next member of tail
            {
                lla->tail->next = NULL;
            }
            
            return nodeToPop;
        }
            
    }
    
    return NULL;        // catch-alla return nulla
}

// TODO: This function was never tested, although it seems to work in acp.c
void llaExtend(struct LinkedListArray *lla, struct LinkedListArray *ext) {
    if (lla == NULL) { return; }
    else if (ext == NULL) { return; }
    else if (lla->len == 0)
    {
        lla->header = ext->header;
        lla->tail = ext->tail;
    }
    else
    {
        lla->tail->next = ext->header;
        if (ext->header != NULL)
        {
            ext->header->prev = lla->tail;
        }
        if (ext->tail != NULL)
        {
            lla->tail = ext->tail;
        }
    }
    lla->len = lla->len + ext->len;
}

// Print a linked list using printf
void llaPrint(struct LinkedListArray *lla) {
    
    if (lla == NULL)
    {
        printf("\nLinked list is NULL.\n");
        return;
    }
    
    struct NodeArray *ptr;
    //int i;
    printf("\n Length=%d { ", lla->len);
    for (ptr = lla->header; ptr != NULL; ptr = ptr->next) {

        nodeArrayPrint(ptr);
        printf(" <=> ");
    }
    printf("NULL }\n");
}

// Print a node using printf
void nodeArrayPrint(struct NodeArray *node) {

    int i;
    if (node == NULL)
    {
        printf("NULLNODE");
    }
    else if (node->val == NULL)
    {
        printf("NULL");
    }
    else
    {
        printf("[");
        for (i = 0; 
             i < (sizeof(node->val) / sizeof(node->val[0]));
             i++)
        {
            if (i + 1 == (sizeof(node->val) / sizeof(node->val[0])))
            {
                printf("%d", node->val[i]);
            }
            else
            {
                printf("%d,", node->val[i]);
            }
        }
        printf("]");
        if (node->prev) { printf("S"); } else { printf("N"); }
        if (node->next) { printf("S"); } else { printf("N"); }
        
    }

}
