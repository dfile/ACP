// C version of acp code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "typedefs.h"
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "set_t.h"

number LOW;  // the smallest curvature in root quad, used to determine set_t range

set_t *CURVELIST;

long long int ceiling;

time_t t;
char *timestamp;

void printTime()
{
    time(&t);
    timestamp = ctime(&t);
    printf("Time: %s\n", timestamp);
}

/**
 * input:  root (the initial quadruple that defines the packing)
 *         limit (arbitrary limit we don't want to go above)
 * output: ancestors (list of quadruples all below the limit)
 */
void fuchsian(number root[4], number limit) {
    struct LinkedListArray ancestors;
    ancestors.header = NULL;
    ancestors.tail = NULL;
    ancestors.len = 0;

    llaAppend(&ancestors, nodeArrayInitWithArray(root));
    struct NodeArray *n = NULL;
    unsigned long long int count = 0;
    unsigned long long int interval = ceiling / 10;
    unsigned char i;
    number prime[4];
    long long int transformed[4];

    while (ancestors.len > 0)
    {
        ++count;
        n = llaPop(&ancestors);

        transformed[0] = -3 * n->val[0] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[1] = -3 * n->val[1] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[2] = -3 * n->val[2] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[3] = -3 * n->val[3] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);

        if (count % interval == 0)
        {
            printf("%lldth iteration: %lld %lld %lld %lld\n", 
                    count, transformed[0], transformed[1], 
                    transformed[2], transformed[3]);
            fflush(stdout);
        }
        
        for (i = 0; i < 4; i++)
        {
            if ( (n->val[i] < transformed[i]) 
                 && (transformed[i] < limit))
            {
                memcpy(prime, n->val, 4*sizeof(number));
                prime[i] = transformed[i];
                llaPush(&ancestors, nodeArrayInitWithArray(prime));
                if (setAdd(CURVELIST, transformed[i]) < 0)
                {
                    exit(1);
                }
            }
        }

        nodeArrayDestroy(n);
        n = NULL;
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
            if (setAdd(possible, ruple->val[i]) < 0)
            {
                exit(1);
            }
        }
    }

    return possible;
}

// Only returns solutions, not an updated orbit because orbit is actually 
// modified in this function since it's a pointer
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
            // If memcmp finds family[i] in orbit, match will be 0
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
        struct LinkedListArray *newGeneration = transformOrbit(parent->val, orbit);
        llaExtend(ancestors, newGeneration);
        free(newGeneration);
    }
    llaDestroy(ancestors);

    return orbit;

}

set_t* path(set_t *valList, number top) {

    set_t *could = setInitWithRange(LOW, ceiling);

    number i = 0;
    for (i = 0; i < top; i++)
    {
        // Alert user if result of mod is negative (it shouldn't be)
        if ((i % 24) < 0) { printf("mod in path was negative"); }
        if (setExists(valList, (i % 24)))
        {
            if (setAdd(could, i) < 0)
            {
                exit(1);
            }
        }
    }

    return could;

}

ll* compare(set_t *valuesGlobal) {

    ll *missing = (ll *)llaInit();
    number i = 0;
    for (i = LOW; i <= ceiling; i++)
    {
        if (setExists(valuesGlobal, i) && !setExists(CURVELIST, i))
        {
            node *n = (node *)nodeInitWithInt(i);
            llAppend(missing, n);
        }
    }

    return missing;

}

// TODO: use ceiling instead of cap
ll* seek(number root[4], number cap)
{

    number i = 0;
    for (i = 0; i < 4; i++)
    {
        if (setAdd(CURVELIST, root[i]) < 0)
        {
            exit(1);
        }
    } 

    printf("In seek\n");

    printf("Running fuchsian\n");
    fflush(stdout);
    fuchsian(root, cap);
    printf("CURVELIST num_items: "NUMFORM"\n", setGetNumItems(CURVELIST));
    fflush(stdout);

    printTime();
    printf("Running genealogy\n");
    fflush(stdout);
    struct LinkedListArray *admissible = genealogy(root);

    printTime();
    printf("Running valuesOf\n");
    fflush(stdout);
    set_t *valuesOrbit = valuesOf(admissible);

    llaDestroy(admissible);

    printTime();
    printf("Running path\n");
    fflush(stdout);
    set_t *valuesGlobal = path(valuesOrbit, cap);

    setDestroy(valuesOrbit);

    printTime();
    printf("Running compare\n");
    fflush(stdout);
    ll *nope = compare(valuesGlobal);

    setDestroy(valuesGlobal);
    setDestroy(CURVELIST);

    return nope;
}

int main(int argc, char *argv[]) {

    if (argc != 6)
    {
        fprintf(stderr, "ERR: Incorrect number of arguments.\n");
        fprintf(stderr, "Usage: acp curv1 curv2 curv3 curv4 ceiling\n");
        fprintf(stderr, "Example: acp -1 2 2 3 1000\n");
        return 0;
    }

    number root[4] = {0,0,0,0};

    unsigned char arg = 1;
    for (arg = 1; arg < 5; arg++)
    {
        root[arg-1] = (number)strtol(argv[arg], NULL, 10);
    }

    number index = 0;
    LOW = root[0];
    for (index = 0; index < 4; index++)
    {
        if (root[index] < LOW) { LOW = root[index]; }
    }

    ceiling = (long long int)strtoll(argv[5], NULL, 10);
    CURVELIST = setInitWithRange(LOW, ceiling);

    printf("Running seek with root {");
    for (index = 0; index < 4; index++) { printf(NUMFORM",",root[index]); }
    printf("} and ceiling %lld\n", ceiling);

    printTime();

    fflush(stdout);

    ll *results = seek(root, ceiling);
    llPrint(results, 0);

    printTime();

    fflush(stdout);

    llDestroy(results);
    return 0;
}  
