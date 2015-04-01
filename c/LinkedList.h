#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

/**
 *  A node that holds an int and points to two other
 *  nodes.  To be used in a linked list.
 */
struct Node {
    int val;
    struct Node *next;
    struct Node *prev;
};

/**
 *  A doubly linked list implementation. Has header
 *  and tail nodes pointing to the front and back
 *  of the list, respectively.
 */
struct LinkedList {
    struct Node *header;
    struct Node *tail;
    int len;
};

// Create and return an empty linked list.
struct LinkedList* llInit(void);

// Destroy linked list and the nodes it contains
void llDestroy(struct LinkedList *ll);

// Create and return an empty node (value initialized to 0).
struct Node* nodeInit(void);

// Create and return an empty node with value i
struct Node* nodeInitWithInt(int i);

// Destroy a node
void nodeDestroy(struct Node *node);

// Append a node to the back of a linked list.
void llAppend(struct LinkedList *ll, struct Node *node);

// Push a node onto the front of a linked list.
void llPush(struct LinkedList *ll, struct Node *node);

// Remove and return node from front of list.
struct Node* llPop(struct LinkedList *ll);

// Remove and return node from back of list.
struct Node* llPopBack(struct LinkedList *ll);

// Extend a list by another list (extend ll by ext)
void llExtend(struct LinkedList *ll, struct LinkedList *ext);

// Print a linked list from front to back.
void llPrint(struct LinkedList *ll);

#endif
