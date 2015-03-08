#include <stdio.h>      // printf
#include <stdlib.h>     // malloc

struct MyStruct {
    int x;
};

void main(void) {
    
    struct MyStruct mystruct1;                                        // MyStruct
    struct MyStruct *mystruct2 = malloc(sizeof(struct MyStruct));     // Pointer to a MyStruct
    // Pointers to structs need to be initialized with malloc
    struct MyStruct *mystruct3 = {5};

    mystruct1.x = 10;       // Use dot notation for struct variables
    mystruct2->x = 2;       // Use arrow notation when struct variable is a pointer

    printf("mystruct1.x = %d \n", mystruct1.x);
    printf("mystruct2->x = %d \n", mystruct2->x);
    printf("mystruct3->x = %d \n", mystruct3->x);
}
