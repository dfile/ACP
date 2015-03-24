#ifndef LINKEDLISTARRAY_H_
#define LINKEDLISTARRAY_H_

/**
 *  A node that holds an array of four ints and points
 *  to two other nodes. To be used in a linked list.
 */
struct NodeArray {
    int val[4];
    struct NodeArray *next;
    struct NodeArray *prev;
};

/**
 *  A doubly linked list implementation. Has header
 *  and tail nodes pointing to the front and back
 *  of the list, respectively.
 */
struct LinkedListArray {
    struct NodeArray *header;
    struct NodeArray *tail;
    int len;
};

// Create and return an empty linked list.
struct LinkedListArray* llaInit(void);

// Create and return an empty node (value initialized to 0).
struct NodeArray* nodeArrayInit(void);

// Create and return a node with an array of 4 numbers.
struct NodeArray* nodeArrayInitWithArray(int arr[4]);

// Append a node to the back of a linked list.
void llaAppend(struct LinkedListArray *lla, struct NodeArray *node);

// Push a node onto the front of a linked list.
void llaPush(struct LinkedListArray *lla, struct NodeArray *node);

// Remove and return node from front of list.
struct NodeArray* llaPop(struct LinkedListArray *lla);

// Remove and return node from back of list.
struct NodeArray* llaPopBack(struct LinkedListArray *lla);

// Extend a list by another list (extend lla by ext)
void llaExtend(struct LinkedListArray *lla, struct LinkedListArray *ext);

// Print a linked list from front to back.
void llaPrint(struct LinkedListArray *lla);

// Print a node's array
void nodeArrayPrint(struct NodeArray *node);

#endif
