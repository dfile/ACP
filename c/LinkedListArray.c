#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedListArray.h"
#include "typedefs.h"

// Create and return an empty linked list
struct LinkedListArray* llaInit(void) {

    //printf("malloc LinkedListArray struct: %d bytes\n", sizeof(struct LinkedListArray));
    struct LinkedListArray *list = malloc(sizeof(struct LinkedListArray));

    if (list == NULL)
    {
        fprintf(stderr, "ERR: couldn't malloc in llaInit()\n");
        exit(1);
        return NULL;
    }

    memset(list, 0, sizeof(struct LinkedListArray));

    return list;
}

// Destroy a linked list and the nodes it contains
void llaDestroy(struct LinkedListArray *lla)
{
    if (lla == NULL) { return; }
    else if (lla->len != 0)
    {
        // delete each node in list
        struct NodeArray *temp;
        while (lla->len != 0)
        {
            temp = llaPop(lla);
            nodeArrayDestroy(temp);
            temp = NULL;
        }
    }
    //printf("Destroy LinkedListArray struct\n");
    free(lla);
}

// Create and return an empty node
// Array is initialized with null elements.
struct NodeArray* nodeArrayInit(void) {
    struct NodeArray *node = nodeArrayInitWithArray(NULL);
    if (node == NULL)
    {
        fprintf(stderr, "ERR: Couldn't malloc in nodeArrayInit()\n");
        exit(1);
    }
    return node;
}

// Create and return a node initialized with given array
struct NodeArray* nodeArrayInitWithArray(number arr[4]) {

    //printf("malloc NodeArray struct: %d bytes\n", sizeof(struct NodeArray));
    struct NodeArray *node = malloc(sizeof(struct NodeArray)); 

    if (node == NULL)
    {
        fprintf(stderr, "ERR: couldn't malloc in nodeArrayInitWithArray()\n");
        exit(1);
        return NULL;
    }
    memset(node, 0, sizeof(struct NodeArray));

    if (arr != NULL)
    {
        memcpy(node->val, arr, 4*sizeof(number));
    }

    return node;
}

// Free a nodeArray and the array it contains
void nodeArrayDestroy(struct NodeArray *node)
{
    if (node == NULL) { return; }
    //printf("destroy NodeArray struct\n");
    free(node);
}

// Push node on front of list
void llaPush(struct LinkedListArray *lla, struct NodeArray *node) {

    // Make sure list and node are not null
    if (lla != NULL && node != NULL) {

        node->next = NULL;
        node->prev = NULL;
        ++(lla->len);

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
            node->next = NULL;        // ensure last node is last
            node->prev = lla->tail;    // node's prev node is tail
            lla->tail->next = node;
            lla->tail = node;
        }
    }
}

// Remove and return node from front of list
struct NodeArray* llaPop(struct LinkedListArray *lla) {

    // Make sure list is not null
    if (lla != NULL) {

        if (lla->header != NULL)  // lla is not empty
        {
            lla->len = lla->len - 1;

            struct NodeArray *nodeToPop = lla->header;
            lla->header = lla->header->next;

            nodeToPop->prev = NULL;
            nodeToPop->next = NULL;

            if (lla->header == NULL)    // nodeToPop was only element in lla
            {
                lla->tail = NULL;
            }
            else
            {
                // nodeToPop wasn't only element, make header
                // first node in list
                lla->header->prev = NULL;
            }

            return nodeToPop;
        }

    }

    return NULL;        // catch-all return null
}

// Remove and return node from back of list
struct NodeArray* llaPopBack(struct LinkedListArray *lla) {

    // Make sure list is not null
    if (lla != NULL) {

        if (lla->tail != NULL)  // lla is not empty
        {
            lla->len = lla->len - 1;

            struct NodeArray *nodeToPop = lla->tail;
            //            struct NodeArray *nodeToPop = nodeArrayInitWithArray(lla->tail->val);
            lla->tail = lla->tail->prev;

            nodeToPop->prev = NULL;
            nodeToPop->next = NULL;

            if (lla->tail == NULL)    // nodeToPop was only element in lla
            {
                lla->header = NULL;
            }
            else
            {
                // nodeToPop wasn't only element, make tail
                // last node in list
                lla->tail->next = NULL;
            }

            return nodeToPop;
        }

    }

    return NULL;        // catch-all return null
}

// TODO: This function was never tested, although it seems to work in acp.c
void llaExtend(struct LinkedListArray *lla, struct LinkedListArray *ext) {
    if (lla == NULL) { 
        printf("lla is NULL in llaExtend()\n");
        return;
    }
    else if (ext == NULL) {
        printf("lla is NULL in llaExtend()\n");
        return;
    }
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
    //number i;
    printf("\n Length="NUMFORM" { ", lla->len);
    for (ptr = lla->header; ptr != NULL; ptr = ptr->next) {

        nodeArrayPrint(ptr);
        printf(" <=> ");
    }
    printf("NULL }\n");
}

// Print a node using printf
void nodeArrayPrint(struct NodeArray *node) 
{
    char buf[30] = {0};
    nodeArrayToString(node, buf);
    printf("%s", buf);
}

// Create the string representation of a node.
// Ensure buf is large enough to hold the string (>30 bytes)
// Also ensure buf is clear of strings before calling
void nodeArrayToString(struct NodeArray *node, char *buf)
{
    number i;
    if (node == NULL)
    {
        sprintf(buf, "NULLNODE");
    }
    else if (node->val == NULL)
    {
        sprintf(buf, "NULL");
    }
    else
    {
        char temp[30];
        sprintf(buf, "["); // First sprintf clears buf
        for (i = 0; 
             i < (sizeof(node->val) / sizeof(node->val[0]));
             i++)
        {
            if (i + 1 == (sizeof(node->val) / sizeof(node->val[0])))
            {
                sprintf(temp, ""NUMFORM"", node->val[i]);
                strcat(buf, temp);
            }
            else
            {
                sprintf(temp, ""NUMFORM",", node->val[i]);
                strcat(buf, temp);
            }
        }
        strcat(buf, "]");
        if (node->prev) { 
            sprintf(temp, "S"); 
            strcat(buf, temp);
        } else { 
            sprintf(temp, "N"); 
            strcat(buf, temp);
        }

        if (node->next) { 
            sprintf(temp, "S"); 
            strcat(buf, temp);
        } else { 
            sprintf(temp, "N"); 
            strcat(buf, temp);
        }
    }
}
