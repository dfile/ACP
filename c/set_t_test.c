// A test for set_t

#include <stdio.h>
#include "set_t.h"

int main(void)
{
    // create a set that holds 0-9
    set_t *s = setInitWithLenAndZero(10, 0);
    printf("Created a new set with zero at 0 and length 10\n");
    setPrint(s,1);
    setPrint(s,0);

    setAdd(s,5);
    setAdd(s,6);
    setAdd(s,0);

    printf("Added three numbers to set\n");
    setPrint(s,1);
    setPrint(s,0);

    // bad add
    setAdd(s,11);
    printf("Did attempting to add 11 change anything (it shouldn't)?\n");
    setPrint(s,1);
    setPrint(s,0);

    // create a set that holds -2 to 7
    set_t *s1 = setInitWithLenAndZero(10, 2);
    printf("\nCreated a new set with zero at 2 and length 10\n");
    setPrint(s1,2);
    //setPrint(s1,0);
    
    printf("\nAdding numbers to set...\n");
    setAdd(s1,-1);
    setAdd(s1,-2);
    setAdd(s1,0);
    setAdd(s1,7);
    setAdd(s1,5);
    setAdd(s1,8);
    setAdd(s1,9);
    setAdd(s1,1000);
    setAdd(s1,-3);

    setPrint(s1,2);
    //setPrint(s1,0);
    
    printf("\nDoes -1 exist in set?\n");
    printf("%d\n",(int)setExists(s1,-1));

    printf("\nRemoving -1 from set\n");
    setRemove(s1,-1);
    setPrint(s1,2);

    printf("\nNow does -1 exist in set?\n");
    printf("%d\n",(int)setExists(s1,-1));

    printf("\nRemoving 4 from set\n");
    setRemove(s1,4);
    setPrint(s1,2);

    printf("\nRemoving -11 from set\n");
    setRemove(s1,-11);
    setPrint(s1,2);

    printf("\nClearing the set\n");
    setClear(s1);
    setPrint(s1,2);

    // create a set that holds -4 to -1
    set_t *s2 = setInitWithLenAndZero(4, 4);
    printf("\nCreated a new set with zero at 4 and length 4\n");
    setPrint(s2,2);

    printf("\nAdding -4, -2, -1 to set...\n");
    setAdd(s2,-4);
    setAdd(s2,-2);
    setAdd(s2,-1);
    setPrint(s2,2);

    printf("\nDoes -4 exist in set?\n");
    printf("%d\n",(int)setExists(s2,-4));
    
    printf("\nRemoving -4\n");
    setRemove(s2,-4);
    setPrint(s2,2);

    printf("\nNow does -4 exist in set?\n");
    printf("%d\n",(int)setExists(s2,-4));

    printf("\nDoes -1 exist in set?\n");
    printf("%d\n",(int)setExists(s2,-1));
   
    // create a set that holds -2 to 7 using the range initializer
    set_t *s3 = setInitWithRange(-2, 7);
    printf("\nCreated a new set with range of -2 to 10\n");
    setPrint(s3,2);
    
    printf("\nAdding numbers to set...\n");
    setAdd(s3,-1);
    setAdd(s3,-2);
    setAdd(s3,0);
    setAdd(s3,7);
    setAdd(s3,5);
    setAdd(s3,9);
    setAdd(s3,9);
    setAdd(s3,1000);
    setAdd(s3,-3);

    setPrint(s3,2);
    //setPrint(s1,0);
    
    printf("\nDoes -1 exist in set?\n");
    printf("%d\n",(int)setExists(s3,-1));

    printf("\nRemoving -1 from set\n");
    setRemove(s3,-1);
    setPrint(s3,2);

    printf("\nNow does -1 exist in set?\n");
    printf("%d\n",(int)setExists(s3,-1));

    printf("\nRemoving 4 from set\n");
    setRemove(s3,4);
    setPrint(s3,2);

    printf("\nRemoving -11 from set\n");
    setRemove(s3,-11);
    setPrint(s3,2);

    printf("\nClearing the set\n");
    setClear(s3);
    setPrint(s3,2);

    // create a set that holds 2 to 5 using the range initializer
    set_t *s4 = setInitWithRange(2, 5);
    printf("\nCreated a new set with range of 2 to 5\n");
    setPrint(s4,2);
    
    printf("\nAdding 2,5,3,6,1,-2,10 to set\n");
    setAdd(s4,2);
    setAdd(s4,5);
    setAdd(s4,3);
    setAdd(s4,6);
    setAdd(s4,1);
    setAdd(s4,-2);
    setAdd(s4,10);
    setPrint(s4,2);
    
    return 0;
}
