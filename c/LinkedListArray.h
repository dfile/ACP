#ifndef LINKEDLISTARRAY_H_
#define LINKEDLISTARRAY_H_

#include "typedefs.h"

/**
 *  A node that holds an array of four ints and points
 *  to two other nodes. To be used in a linked list.
 */
typedef struct NodeArray {
    number val[4];
    struct NodeArray *next;
    struct NodeArray *prev;
} nodeA;

/**
 *  A doubly linked list implementation. Has header
 *  and tail nodes pointing to the front and back
 *  of the list, respectively.
 */
typedef struct LinkedListArray {
    struct NodeArray *header;
    struct NodeArray *tail;
    number len;
} lla;

// Create and return an empty linked list.
lla*   llaInit(void);

// Destroy linked list and the nodes it contains
void   llaDestroy(struct LinkedListArray *lla);

// Create and return an empty node (value initialized to 0).
nodeA* nodeArrayInit(void);

// Create and return a node with an array of 4 numbers.
nodeA* nodeArrayInitWithArray(number arr[4]);

// Destroy a nodeArray and the array it contains
void   nodeArrayDestroy(struct NodeArray *node);

// Append a node to the back of a linked list.
void   llaAppend(struct LinkedListArray *lla, struct NodeArray *node);

// Push a node onto the front of a linked list.
void   llaPush(struct LinkedListArray *lla, struct NodeArray *node);

// Remove and return node from front of list.
nodeA* llaPop(struct LinkedListArray *lla);

// Remove and return node from back of list.
nodeA* llaPopBack(struct LinkedListArray *lla);

// Extend a list by another list (extend lla by ext)
void   llaExtend(struct LinkedListArray *lla, struct LinkedListArray *ext);

// Print a linked list from front to back.
void   llaPrint(struct LinkedListArray *lla);

// Print a node's array
void   nodeArrayPrint(struct NodeArray *node);

#endif
