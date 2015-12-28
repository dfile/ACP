#include <stdio.h>
#include "LinkedListArray.h"

void main(void) {
    struct NodeArray *soln = nodeArrayInitWithArray(((int [4]){(-1 + (2 * (3 + 4 + 5))), 8, 9, 6}));
    nodeArrayPrint(soln);
    printf("\n");

    int arr[4] = {3,3,3,3};
    struct NodeArray *node = nodeArrayInitWithArray(arr);
    arr[0]=45;
    nodeArrayPrint(node);
    printf("\n");

    node->val[0] = -8;
    nodeArrayPrint(node);
    printf("\n");
}
