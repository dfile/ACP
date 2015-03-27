// C version of acp code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "set_t.h"
//#include "hashsetint.h"

#define LOW -1

set_t *CURVELIST;

int ceiling;

// input: quad (a quadruple of curvatures)
// output: solutions (list of transformed quadruples)
// function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
struct LinkedListArray* transform(int quad[4], int limit) {
    struct LinkedListArray *solutions = llaInit();
    int a = quad[0], b = quad[1], c = quad[2], d = quad[3];
    int twice = (a + b + c + d) << 1; // very quick way to multiply by 2

    int transformed[4] = {0,0,0,0};
    transformed[0] = -3 * a + twice;
    transformed[1] = -3 * b + twice;
    transformed[2] = -3 * c + twice;
    transformed[3] = -3 * d + twice;

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        if (    (quad[i] < transformed[i]) 
             && (quad[i] < limit)
             && (transformed[i] < limit))
        {
            int prime[4];
            memcpy(prime, quad, 4*sizeof(int));
            prime[i] = transformed[i];
            llaAppend(solutions, nodeArrayInitWithArray(prime));
            setAdd(CURVELIST, transformed[i]);
        }
    }

    return solutions;
}

/**
 * input:  root (the initial quadruple that defines the packing)
 *         limit (arbitrary limit we don't want to go above)
 * output: ancestors (list of quadruples all below the limit)
 */
void fuchsian(int root[4], int limit) {
    struct LinkedListArray *ancestors = llaInit();
    llaAppend(ancestors, nodeArrayInitWithArray(root));

    while (ancestors->len > 0)
    {
        //printf("ancestors before extend has len=%d and elements:", ancestors->len);
        //llaPrint(ancestors);
        llaExtend( ancestors, transform((llaPop(ancestors)->val), limit) );
        //printf("ancestors after extend has len=%d and elements:", ancestors->len);
        //llaPrint(ancestors);
        //printf("\n");
    }

    return;
}

set_t* valuesOf(struct LinkedListArray* quadList) {
    set_t *possible = setInitWithRange(LOW, ceiling);
    struct NodeArray *ruple = NULL;
    for (ruple = quadList->header; ruple != NULL; ruple = ruple->next)
    {
        unsigned char i;
        for (i = 0; i < 4; i++)
        {
            setAdd(possible, ruple->val[i]);
        }
    }

    return possible;
}

// Only returns solutions, not an updated orbit because orbit is actually modified in this function since it's a pointer
struct LinkedListArray* transformOrbit(int quad[4], struct LinkedListArray* orbit) {
    struct LinkedListArray *solutions = llaInit();
    int family[4][4];
    int a = quad[0], b = quad[1], c = quad[2], d = quad[3];
 
    int aPos = (-a + (2 * (b + c + d))) % 24;
    int bPos = (-b + (2 * (a + c + d))) % 24;
    int cPos = (-c + (2 * (a + b + d))) % 24;
    int dPos = (-d + (2 * (a + b + c))) % 24;

    // This ensures that the results from mod 24 above are positive
    aPos = aPos < 0 ? aPos + 24 : aPos;
    bPos = bPos < 0 ? bPos + 24 : bPos;
    cPos = cPos < 0 ? cPos + 24 : cPos;
    dPos = dPos < 0 ? dPos + 24 : dPos;

    // the four matrices
    memcpy(family[0], ((int [4]){aPos,   b % 24, c % 24, d % 24}), 4*sizeof(int));
    memcpy(family[1], ((int [4]){a % 24, bPos,   c % 24, d % 24}), 4*sizeof(int));
    memcpy(family[2], ((int [4]){a % 24, b % 24, cPos,   d % 24}), 4*sizeof(int));
    memcpy(family[3], ((int [4]){a % 24, b % 24, c % 24, dPos}),   4*sizeof(int));

    unsigned char i;
    int match = 1;
    for (i = 0; i < 4; i++)
    {
        struct NodeArray *ptr = NULL;
        for (ptr = orbit->header; ptr != NULL; ptr = ptr->next)
        {
            // If memcmp finds family[i] in orbit, match will be 0, so
            // multiplying by 0 will always give 0, and match will end
            // up as 0.

            match = match * memcmp(family[i], ptr->val, 4*sizeof(int));

        }
        
        // If family[i] was not in orbit, append it to orbit
        if (match != 0)
        {
            llaAppend(orbit, nodeArrayInitWithArray(family[i]));
            llaAppend(solutions, nodeArrayInitWithArray(family[i]));
        }
        
        match = 1;
    }

    return solutions;

}

struct LinkedListArray* genealogy(int seed[4]) {
    struct LinkedListArray *ancestors = llaInit();
    struct LinkedListArray *orbit = llaInit();
    int modSeed[4] = {0};

    unsigned char i;
    for (i = 0; i < 4; i++)
    {
        if ((modSeed[i] = (seed[i] % 24)) < 0)
        {
            modSeed[i] = modSeed[i] + 24;
        }
    }

    llaAppend(ancestors, nodeArrayInitWithArray(modSeed));
    llaAppend(orbit, nodeArrayInitWithArray(modSeed));
    
    struct NodeArray *parent = NULL;
    for (parent = ancestors->header; parent != NULL; parent = parent->next)
    {
        struct LinkedListArray *newGeneration = transformOrbit(parent->val, orbit);
        llaExtend(ancestors, newGeneration);
    }

    return orbit;

}

// FIXME: something is wrong in here or in a previous function
set_t* path(set_t *valList, int top) {

    set_t *could = setInitWithRange(LOW, ceiling);

    int i = 0;
    for (i = 0; i < top; i++)
    {
        // int j = 0;
        // for (j = LOW; j < ceiling; j++)
        // {
            // TODO: does this mod really do what I think it does? (check if negative)
            if ((i % 24) < 0) { printf("mod in path was negative"); }
            if (setExists(valList, (i % 24)))
            {
                setAdd(could, i);
                // break;
            }
        // }
    }

    return could;

}

set_t* compare(set_t *valuesGlobal) {

    set_t *missing = setInitWithRange(LOW, ceiling);
    int i = 0;
    for (i = LOW; i < ceiling; i++)
    {
        printf("valuesGlobal[%d] = %d  :  CURVELIST[%d] = %d", i, setExists(valuesGlobal, i), i, setExists(CURVELIST, i));
        // if ((valuesGlobal[i] != 0) && (valuesGlobal[i] != CURVELIST[i]))
        if (setExists(valuesGlobal, i) && !setExists(CURVELIST, i))
        {
            printf(" - unique to valuesGlobal, adding to missing\n");
            //missing[i] = 1;
            setAdd(missing, i);
        }
        else
        {
            printf(" - not added to missing\n");
        }
    }

    return missing;

}

set_t* seek(int root[4], int cap)
{

    int globalCount = 0;
    for (globalCount = 0; globalCount < 4; globalCount++)
    {
        // CURVELIST[root[globalCount] + ZERO] = 1;
        setAdd(CURVELIST, root[globalCount]);
    } 
    
    //printf("In seek\n");
    fuchsian(root, cap);
    //printf("  fuchsian results - matches with acp python code \n");
    //llaPrint(small);
    //struct LinkedList *valuesPack = valuesOf(small);
    //printf("  valuesOf results from fuchsian - matches with acp python code \n");
    //llPrint(valuesPack);
    struct LinkedListArray *admissible = genealogy(root);
    //printf("  genealogy results - matches with acp python code\n");
    //llaPrint(admissible);
    set_t *valuesOrbit = valuesOf(admissible);
    //printf("  valuesOf results from genealogy - mathces with acp python code\n");
    //llPrint(valuesOrbit);
    set_t *valuesGlobal = path(valuesOrbit, cap);
    set_t *nope = compare(valuesGlobal);
/*
    // FIXME: nope is way off. check path.
    printf("nope: {\n");
    for (int globalCount = 0; globalCount < (ceiling + ZERO); globalCount++)
    {
        printf("index: %d  value: %d\n", globalCount, (char)nope[globalCount]);
    }
    printf("}\n");

    // CURVELIST looks correct
    printf("CURVELIST: {\n");
    for (int globalCount = 0; globalCount < (ceiling + ZERO); globalCount++)
    {
        printf("index: %d  value: %d\n", globalCount, (char)CURVELIST[globalCount]);
    }
    printf("}\n");

    struct LinkedList *missing = llInit();
    int i = 0;
    for (int globalCount = 0; globalCount < (ceiling + ZERO); globalCount++)
    {
        if (nope[globalCount] == 1)
        {
            struct Node *node = nodeInit();
            node->val = i - ZERO;
            llAppend(missing, node);
            free(node);
        }
    }

    return missing;

*/
    return nope;
}

int main(void) {

    int root[4] = {-1, 2, 2, 3};
    int index = 0;
    ceiling = 1000;
    CURVELIST = setInitWithRange(LOW, ceiling);

    printf("Running seek with root {");
    for (index = 0; index < 4; index++) { printf("%d,",root[index]); }
    printf("} and ceiling %d\n", ceiling);

    set_t *results = seek(root, ceiling);
    setPrint(results, 0);
    //struct LinkedList *results = seek(root, ceiling);
    //llPrint(results);

    return 0;
}  
