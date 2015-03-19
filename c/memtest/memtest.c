#include <stdio.h>
#include <stdlib.h>
//#include "LinkedListArray.h"
#include "LinkedList.h"

struct LinkedList* test() {
    struct LinkedList *testLL = llInit();
    return testLL;
}

void main(void) {
    int i = 0;
    struct LinkedList *ll = test();
    
    for (i = 0; i < 2; i++) {
        struct Node *node = nodeInit();
        node->val = i;
        llAppend(ll, node);
        free(node);
    }
    
    llPrint(ll);
    free(ll);
}
