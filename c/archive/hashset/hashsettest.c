#include <stdio.h>
#include "hashsetint.h"

void main(void) {

    int a = 5;
    hashset_t set = hashset_create();

    int exists = hashset_add(set, a);
    printf("%d did not exist: %d\n", a, exists);

    exists = hashset_is_member(set, a);
    printf("%d now exists: %d\n", a, exists);

    int b = 5;
    exists = hashset_is_member(set, b);
    printf("A different version of %d exists: %d\n", b, exists);

    //int *c = &a;
    //exists = hashset_is_member(set, *c);
    //printf("Pointer to %d exists: %d\n", *c, exists);
    
    int d = 6;
    exists = hashset_is_member(set, d);
    printf("Does %d exist? %d\n", d, exists);
    
    exists = hashset_add(set, d);
    printf("%d did not exist: %d\n", d, exists);

}
