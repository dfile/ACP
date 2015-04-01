// C version of acp code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "set_t.h"

int LOW;

set_t *CURVELIST;

int ceiling;

// input: quad (a quadruple of curvatures)
// output: solutions (list of transformed quadruples)
// function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
struct LinkedListArray* transform(int quad[4], int limit) {
//void transform(int quad[4], int limit, struct LinkedListArray *solutions) {
    struct LinkedListArray *solutions = llaInit();
    //struct LinkedListArray solutions;
    //solutions.header = NULL;
    //solutions.tail = NULL;
    //solutions.len = 0;

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
    //struct LinkedListArray *ancestors = llaInit();
    struct LinkedListArray ancestors;
    ancestors.header = NULL;
    ancestors.tail = NULL;
    ancestors.len = 0;

    llaAppend(&ancestors, nodeArrayInitWithArray(root));

    while (ancestors.len > 0)
    {
        struct NodeArray *n = llaPop(&ancestors);
        //int a[4];
        //memcpy(a, n->val, 4*sizeof(int));
        struct LinkedListArray *l = transform(n->val, limit);
        //struct LinkedListArray l;
        //l.header = NULL;
        //l.tail = NULL;
        //l.len = 0;

        //transform(a, limit, &l);
        //llaPrint(l);
        //llaExtend( &ancestors, l );
       
        //printf("%d\n", l->header);
        //printf("%d\n", l->tail);
        //printf("%d\n", l->len);
        while (1)
        {
            struct NodeArray *ptr = NULL;
            ptr = llaPop(l);
            if (ptr == NULL) { break; }
            llaAppend(&ancestors, ptr);
            //nodeArrayDestroy(ptr);
        }
        //llaPrint(l);
        //printf("%d\n", l->header);
        //printf("%d\n", l->tail);
        //printf("%d\n", l->len);
        free(l);
        //llaDestroy(l);
        nodeArrayDestroy(n);
    }
    //llaDestroy(ancestors);

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
        free(newGeneration);
    }
    llaDestroy(ancestors);

    return orbit;

}

set_t* path(set_t *valList, int top) {

    set_t *could = setInitWithRange(LOW, ceiling);

    int i = 0;
    for (i = 0; i < top; i++)
    {
            // Alert user if result of mod is negative (it shouldn't be)
            if ((i % 24) < 0) { printf("mod in path was negative"); }
            if (setExists(valList, (i % 24)))
            {
                setAdd(could, i);
            }
    }

    return could;

}

set_t* compare(set_t *valuesGlobal) {

    set_t *missing = setInitWithRange(LOW, ceiling);
    int i = 0;
    for (i = LOW; i <= ceiling; i++)
    {
        if (setExists(valuesGlobal, i) && !setExists(CURVELIST, i))
        {
            setAdd(missing, i);
        }
    }

    return missing;

}

set_t* seek(int root[4], int cap)
{

    int globalCount = 0;
    for (globalCount = 0; globalCount < 4; globalCount++)
    {
        setAdd(CURVELIST, root[globalCount]);
    } 
    
    printf("In seek\n");

    printf("Running fuchsian\n");
    fuchsian(root, cap);
    printf("CURVELIST num_items: %d\n", setGetNumItems(CURVELIST));
    //setPrint(CURVELIST, 0);

    printf("Running genealogy\n");
    struct LinkedListArray *admissible = genealogy(root);
    //llaPrint(admissible);

    printf("Running valuesOf\n");
    set_t *valuesOrbit = valuesOf(admissible);
    //setPrint(valuesOrbit, 0);

    llaDestroy(admissible);

    printf("Running path\n");
    set_t *valuesGlobal = path(valuesOrbit, cap);

    setDestroy(valuesOrbit);

    printf("Running compare\n");
    set_t *nope = compare(valuesGlobal);

    setDestroy(valuesGlobal);
    setDestroy(CURVELIST);

    return nope;

    //return NULL;
}

int main(void) {

    int root[4] = {-1, 2, 2, 3};
    int index = 0;
    ceiling = 50000000;
    LOW = -1;  // the smallest curvature in root quad, used to determine set_t range
    CURVELIST = setInitWithRange(LOW, ceiling);

    printf("Running seek with root {");
    for (index = 0; index < 4; index++) { printf("%d,",root[index]); }
    printf("} and ceiling %d\n", ceiling);

    set_t *results = seek(root, ceiling);
    setPrint(results, 0);
    
    setDestroy(results);
    return 0;
}  
