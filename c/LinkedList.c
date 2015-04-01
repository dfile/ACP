#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

// Create and return an empty linked list
struct LinkedList* llInit(void) {
    struct LinkedList *list = malloc(sizeof(struct LinkedList));

    if (list == NULL)
    {
        printf("couldn't malloc in llInit()\n");
        return NULL;
    }

    list->header = NULL;
    list->tail = NULL;
    return list;
}

// Destroy a linked list and the nodes it contains
void llDestroy(struct LinkedList *ll)
{
    if (ll == NULL) { return; }
    else if (ll->len != 0)
    {
        // delete each node in list
        int i = 0;
        struct Node *temp;
        for (i = 0; i < ll->len; i++)
        {
            temp = llPop(ll);
            nodeDestroy(temp);
        }
    }
    free(ll->header);
    free(ll->tail);
    free(ll);
}

// Create and return an empty node
struct Node* nodeInit(void) {
    struct Node *node = nodeInitWithInt(0);
    
    if (node == NULL)
    {
        printf("Couldn't malloc in nodeInit()\n");
    }

    return node;
}

// Create and return a node initialized with an int
struct Node* nodeInitWithInt(int i) {
    struct Node *node = malloc(sizeof(struct Node));

    if (node == NULL)
    {
        printf("couldn't malloc in nodeInitWithInt()\n");
        return NULL;
    }

    node->val = i;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Free a node
void nodeDestroy(struct Node *node)
{
    if (node == NULL) { return; }
    free(node->prev);
    free(node->next);
    free(node);
}

// Push node on front of list
void llPush(struct LinkedList *ll, struct Node *node) {
    
    // Make sure list and node are not null
    if (ll != NULL && node != NULL) {
        
        node->next = NULL;
        node->prev = NULL;
        ll->len = ll->len + 1;

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
        ll->len = ll->len + 1;
        
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
            ll->len = ll->len - 1;

            struct Node *nodeToPop = ll->header;
            ll->header = ll->header->next;

            nodeToPop->prev = NULL;
            nodeToPop->next = NULL;
            
            if (ll->header == NULL)    // nodeToPop was only element in ll
            {
                ll->tail = NULL;
            }
            else
            {
                // nodeToPop wasn't only element, make header
                // first node in list
                ll->header->prev = NULL;
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
            ll->len = ll->len - 1;
            struct Node *nodeToPop = ll->tail;
            ll->tail = ll->tail->prev;
            ll->tail->next = NULL;

            nodeToPop->prev = NULL;
            nodeToPop->next = NULL;
            
            if (ll->tail == NULL)    // nodeToPop was only element in ll
            {
                ll->header = NULL;
            }
            else
            {
                // nodeToPop wasn't only element, make tail
                // last node in list
                ll->tail->next = NULL;
            }
            
            return nodeToPop;
        }
            
    }
    
    return NULL;        // catch-all return null
}

// XXX: This function was never tested!
void llExtend(struct LinkedList *ll, struct LinkedList *ext) {
    if (ll == NULL) { return; }
    else if (ext == NULL) { return; }
    else if (ll->len == 0)
    {
        ll->header = ext->header;
        ll->tail = ext->tail;
    }
    else
    {
        ll->tail->next = ext->header;
        if (ext->header != NULL)
        {
            ext->header->prev = ll->tail;
        }
        if (ext->tail != NULL)
        {
            ll->tail = ext->tail;
        }
    }
    ll->len = ll->len + ext->len;
}

// Print a linked list using printf
void llPrint(struct LinkedList *ll) {
    
    if (ll == NULL)
    {
        printf("\nLinked list is NULL.\n");
        return;
    }
    
    struct Node *ptr;
    printf("\n Length=%d [ ", ll->len);
    for (ptr = ll->header; ptr != NULL; ptr = ptr->next) {
        printf("(%d ", ptr->val);
        if (ptr->prev == NULL) { printf("N"); }
        else { printf("S"); }
        if (ptr->next == NULL) { printf("N) "); }
        else { printf("S) "); }
    }
    printf("NULL ]\n");
}
