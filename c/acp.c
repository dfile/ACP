// C version of acp code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "set_t.h"

//typedef int number;

number LOW;  // the smallest curvature in root quad, used to determine set_t range

set_t *CURVELIST;

number ceiling;

// input: quad (a quadruple of curvatures)
// output: solutions (list of transformed quadruples)
// function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
//struct LinkedListArray* transform(number quad[4], number limit) {
//XXX: not used
void transform(number quad[4], number limit, struct LinkedListArray *solutions) {
    //struct LinkedListArray *solutions = llaInit();
    //struct LinkedListArray solutions;
    //memset(solutions, 0, sizeof(lla));
    //solutions.header = NULL;
    //solutions.tail = NULL;
    //solutions.len = 0;

    //number a = quad[0], b = quad[1], c = quad[2], d = quad[3];
    //number twice = (a + b + c + d) << 1; // very quick way to multiply by 2

    //number transformed[4] = {0,0,0,0};
    number transformed[4] = {-3 * quad[0] + ((quad[0] + quad[1] + quad[2] + quad[3]) << 1),
                             -3 * quad[1] + ((quad[0] + quad[1] + quad[2] + quad[3]) << 1),
                             -3 * quad[2] + ((quad[0] + quad[1] + quad[2] + quad[3]) << 1),
                             -3 * quad[3] + ((quad[0] + quad[1] + quad[2] + quad[3]) << 1)};
    //transformed[0] = -3 * a + twice;
    //transformed[1] = -3 * b + twice;
    //transformed[2] = -3 * c + twice;
    //transformed[3] = -3 * d + twice;

    unsigned char i = 0;
    for (i = 0; i < 4; i++)
    {
        if (    (quad[i] < transformed[i]) 
                && (quad[i] < limit)
                && (transformed[i] < limit))
        {
            number prime[4];
            memcpy(prime, quad, 4*sizeof(number));
            prime[i] = transformed[i];
            llaAppend(solutions, nodeArrayInitWithArray(prime));
            setAdd(CURVELIST, transformed[i]);
        }
    }

    //return solutions;
    return;
}

/**
 * input:  root (the initial quadruple that defines the packing)
 *         limit (arbitrary limit we don't want to go above)
 * output: ancestors (list of quadruples all below the limit)
 */
void fuchsian(number root[4], number limit) {
    //struct LinkedListArray *ancestors = llaInit();
    struct LinkedListArray ancestors;
    ancestors.header = NULL;
    ancestors.tail = NULL;
    ancestors.len = 0;

    llaAppend(&ancestors, nodeArrayInitWithArray(root));

    while (ancestors.len > 0)
    {
        struct NodeArray *n = llaPop(&ancestors);
        //number a[4];
        //memcpy(a, n->val, 4*sizeof(number));
        //struct LinkedListArray *l = transform(n->val, limit);
        //struct LinkedListArray l;
        //l.header = NULL;
        //l.tail = NULL;
        //l.len = 0;
        //memset(&l, 0, sizeof(lla));

        long long int transformed[4] = {-3 * n->val[0] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1),
                                 -3 * n->val[1] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1),
                                 -3 * n->val[2] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1),
                                 -3 * n->val[3] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1)};
        
        unsigned char i;
        for (i = 0; i < 4; i++)
        {
            if ( (n->val[i] < transformed[i]) 
                 && (transformed[i] < limit))
            {
                number prime[4];
                memcpy(prime, n->val, 4*sizeof(number));
                prime[i] = transformed[i];
                llaPush(&ancestors, nodeArrayInitWithArray(prime));
                setAdd(CURVELIST, transformed[i]);
            }
        }


        //transform(n->val, limit, &l);
        //llaPrint(&l);
        //llaExtend( &ancestors, &l );

        //printf(NUMFORM"\n", l->header);
        //printf(NUMFORM"\n", l->tail);
        //printf(NUMFORM"\n", l->len);
        /*
        while (1)
        {
            //printf("Looped\n");
            struct NodeArray *ptr = NULL;
            ptr = llaPop(&l);
            //nodeArrayPrint(ptr);
            if (ptr == NULL) { break; }
            llaAppend(&ancestors, ptr);
            //nodeArrayDestroy(ptr);
        }
        */
        //printf("Broke loop\n");
        //llaPrint(l);
        //printf(NUMFORM"\n", l->header);
        //printf(NUMFORM"\n", l->tail);
        //printf(NUMFORM"\n", l->len);
        //free(l);
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

    //setPrint(possible, 0);

    return possible;
}

// Only returns solutions, not an updated orbit because orbit is actually modified in this function since it's a pointer
struct LinkedListArray* transformOrbit(number quad[4], struct LinkedListArray* orbit) {
    struct LinkedListArray *solutions = llaInit();
    number family[4][4];
    number a = quad[0], b = quad[1], c = quad[2], d = quad[3];

    number aPos = (-a + (2 * (b + c + d))) % 24;
    number bPos = (-b + (2 * (a + c + d))) % 24;
    number cPos = (-c + (2 * (a + b + d))) % 24;
    number dPos = (-d + (2 * (a + b + c))) % 24;

    // This ensures that the results from mod 24 above are positive
    aPos = aPos < 0 ? aPos + 24 : aPos;
    bPos = bPos < 0 ? bPos + 24 : bPos;
    cPos = cPos < 0 ? cPos + 24 : cPos;
    dPos = dPos < 0 ? dPos + 24 : dPos;

    // the four matrices
    memcpy(family[0], ((number [4]){aPos,   b % 24, c % 24, d % 24}), 4*sizeof(number));
    memcpy(family[1], ((number [4]){a % 24, bPos,   c % 24, d % 24}), 4*sizeof(number));
    memcpy(family[2], ((number [4]){a % 24, b % 24, cPos,   d % 24}), 4*sizeof(number));
    memcpy(family[3], ((number [4]){a % 24, b % 24, c % 24, dPos}),   4*sizeof(number));

    unsigned char i;
    for (i = 0; i < 4; i++)
    {
        struct NodeArray *ptr = NULL;
        unsigned int matched = 0;
        int match = 1;
        for (ptr = orbit->header; ptr != NULL; ptr = ptr->next)
        {
            // If memcmp finds family[i] in orbit, match will be 0, so
            // multiplying by 0 will always give 0, and match will end
            // up as 0.

            match = memcmp(family[i], ptr->val, 4*sizeof(number));
            if (match == 0)
            {
                matched = 1;
                break;
            }

        }

        // If family[i] was not in orbit, append it to orbit
        if (matched == 0)
        {
            llaAppend(orbit, nodeArrayInitWithArray(family[i]));
            llaAppend(solutions, nodeArrayInitWithArray(family[i]));
        }
        else
        {
            int j;
            //printf("[ ");
            for (j = 0; j < 4; j++)
            {
                //printf("%d, ", family[i][j]);
            }
            //printf("] was in orbit: ");
            //llaPrint(orbit);
        }
    }

    return solutions;

}

struct LinkedListArray* genealogy(number seed[4]) {
    struct LinkedListArray *ancestors = llaInit();
    struct LinkedListArray *orbit = llaInit();
    number modSeed[4] = {0};

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
        //printf("Running transformOrbit\n");
        struct LinkedListArray *newGeneration = transformOrbit(parent->val, orbit);
        //printf("newGeneration: ");
        //llaPrint(newGeneration);
        llaExtend(ancestors, newGeneration);
        free(newGeneration);
    }
    llaDestroy(ancestors);
    //printf("orbit at end of genealogy: ");
    //llaPrint(orbit);

    return orbit;

}

set_t* path(set_t *valList, number top) {

    set_t *could = setInitWithRange(LOW, ceiling);

    number i = 0;
    for (i = 0; i < top; i++)
    {
        // Alert user if result of mod is negative (it shouldn't be)
        if ((i % 24) < 0) { printf("mod in path was negative"); }
        if (i == 834) {
            //printf("i is 834 in path()\n");
            if (setExists(valList, (i % 24))) {
                //printf("834 %% 24 is in valList (good!)\n");
            }
            else {
                //printf("834 %% 24 is not in valList (bad)\n");
                //printf("834 %% 24 = %d\n", 834 % 24);
                //setPrint(valList, 0);
            }
        }
        if (setExists(valList, (i % 24)))
        {
            setAdd(could, i);
            if (i == 834) {
                //printf("Was 834 added to valList? ");
                if (setExists(could, i)) {
                    //printf("Yes (good!)\n");
                }
                else {
                    //printf("No (bad)\n");
                }
            }
        }
    }

    return could;

}

//set_t* compare(set_t *valuesGlobal) {
ll* compare(set_t *valuesGlobal) {

    //set_t *missing = setInitWithRange(LOW, ceiling);
    ll *missing = (ll *)llaInit();
    number i = 0;
    for (i = LOW; i <= ceiling; i++)
    {
        if (i == 834) {
            //printf("Comparing "NUMFORM" in compare() (good!)\n", i);
            if (setExists(valuesGlobal, i)) {
                //printf("834 is in valuesGlobal (good!)\n");
            }
            else {
                //printf("834 is not in valuesGlobal (bad)\n");
            }
            if (!setExists(CURVELIST, i)) {
                //printf("834 is not in CURVELIST (good!)\n");
            }
            else {
                //printf("834 is in CURVELIST (bad)\n");
            }
            //setPrint(valuesGlobal, 0);
            //setPrint(CURVELIST, 0);
        }
        if (setExists(valuesGlobal, i) && !setExists(CURVELIST, i))
        {
            //if (i == 834) { printf("Adding "NUMFORM" to missing in compare() (good!)\n", i); }
            node *n = (node *)nodeInitWithInt(i);
            //setAdd(missing, i);
            llAppend(missing, n);
        }
    }

    return missing;

}

// TODO: use ceiling instead of cap
//set_t* seek(number root[4], number cap)
ll* seek(number root[4], number cap)
{

    number i = 0;
    for (i = 0; i < 4; i++)
    {
        setAdd(CURVELIST, root[i]);
    } 

    printf("In seek\n");

    printf("Running fuchsian\n");
    fflush(stdout);
    fuchsian(root, cap);
    printf("CURVELIST num_items: "NUMFORM"\n", setGetNumItems(CURVELIST));
    fflush(stdout);
    //setPrint(CURVELIST, 0);

    printf("Running genealogy\n");
    fflush(stdout);
    struct LinkedListArray *admissible = genealogy(root);
    //llaPrint(admissible);

    printf("Running valuesOf\n");
    fflush(stdout);
    set_t *valuesOrbit = valuesOf(admissible);
    //setPrint(valuesOrbit, 0);

    llaDestroy(admissible);

    printf("Running path\n");
    fflush(stdout);
    set_t *valuesGlobal = path(valuesOrbit, cap);

    setDestroy(valuesOrbit);

    printf("Running compare\n");
    fflush(stdout);
    //set_t *nope = compare(valuesGlobal);
    ll *nope = compare(valuesGlobal);

    setDestroy(valuesGlobal);
    setDestroy(CURVELIST);

    return nope;

    //return NULL;
}

int main(int argc, char *argv[]) {

    number root[4] = {-1, 2, 2, 3};

    number index = 0;
    LOW = root[0];
    for (index = 0; index < 4; index++)
    {
        if (root[index] < LOW) { LOW = root[index]; }
    }

    ceiling = 1000;
    if (argc > 1) { ceiling = atoi(argv[1]); }
    CURVELIST = setInitWithRange(LOW, ceiling);

    printf("Running seek with root {");
    for (index = 0; index < 4; index++) { printf(NUMFORM",",root[index]); }
    printf("} and ceiling "NUMFORM"\n", ceiling);
    fflush(stdout);

    //set_t *results = seek(root, ceiling);
    ll *results = seek(root, ceiling);
    //setPrint(results, 0);
    llPrint(results);
    fflush(stdout);

    //setDestroy(results);
    llDestroy(results);
    return 0;
}  
