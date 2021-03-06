// C version of acp code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "typedefs.h"
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "set_t.h"

// the smallest curvature in root quad, used to determine set_t range
number LOW;  

set_t *CURVELIST;

long long int ceiling;

time_t t;
time_t start_time;
time_t end_time;
char *timestamp;

pthread_mutex_t write_lock = PTHREAD_MUTEX_INITIALIZER;

void printTime()
{
    time(&t);
    timestamp = ctime(&t);
    printf("Time: %s\n", timestamp);
}

void* fuchsianThread(void *args)
{
    time_t thread_start_time;
    time_t thread_end_time;

    time(&thread_start_time);

    number quad[4];
    memcpy(quad, args, 4 * sizeof(number));
    free(args);

    struct LinkedListArray ancestors;
    ancestors.header = NULL;
    ancestors.tail = NULL;
    ancestors.len = 0;

    nodeA *rootNode = nodeArrayInitWithArray(quad);

#if DEBUGGING
    rootNode->lvl = 0;
#endif
    
    llaAppend(&ancestors, rootNode);
    struct NodeArray *n = NULL;
    unsigned char i;
    number prime[4];
    long long int transformed[4];

    pthread_t self = pthread_self();

#if DEBUGGING

    // XXX: This buffer is a potential segfault. Just know that
    // if this run has unusually long output, it will
    // cause this to segfault.
    char buf[1000*1000]; // 1 MB buffer
    memset(buf, 0, sizeof(1000*1000));
    buf[0] = '\0'; // Allows use of strcat
    sprintf(buf, "Thread %u\n", (unsigned int)self);
#endif

    while (ancestors.len > 0)
    {
        n = llaPop(&ancestors);

#if DEBUGGING
        char temp[1000];
        memset(temp, 0, 1000);

        unsigned int numTabs = 0;

        for (; numTabs < n->lvl; numTabs++) 
        { 
            strcat(buf, "|  "); 
        }
        nodeArrayToString(n, temp);
        strcat(buf, temp);
        strcat(buf, "\n");
#endif

        transformed[0] = 
                -3 * n->val[0] 
                + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[1] = 
                -3 * n->val[1] 
                + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[2] = 
                -3 * n->val[2] 
                + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[3] = 
                -3 * n->val[3] 
                + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        
        for (i = 0; i < 4; i++)
        {

#if DEBUGGING
            if (i == 0) {
                printf("Transformed: %lld %lld %lld %lld\n"
                        "Root: %lld %lld %lld %lld\n",
                        transformed[0], transformed[1],
                        transformed[2], transformed[3],
                        n->val[0],      n->val[1],
                        n->val[2],      n->val[3]);
            }
#endif
            if ( (n->val[i] < transformed[i]) 
                 && (transformed[i] < ceiling))
            {
                memcpy(prime, n->val, 4*sizeof(number));
                prime[i] = transformed[i];
                nodeA *primeNode = nodeArrayInitWithArray(prime);
#if DEBUGGING
                primeNode->lvl = n->lvl + 1;
#endif
                llaPush(&ancestors, primeNode);
                unumber index = setOffsetToZero(CURVELIST, transformed[i]);
                unumber i = index / 8;
                byte pos = index % 8;
                byte mask = 1 << pos;
                // atomically check and set bit if bit is not set
                __sync_bool_compare_and_swap(CURVELIST->items + i,        // value to check
                                             CURVELIST->items[i] & ~mask,  // old value
                                             CURVELIST->items[i] | mask); // new value
            }
        }
        nodeArrayDestroy(n);
        n = NULL;
    }

#if DEBUGGING
    printf("%s\n", buf);
#endif

    time(&thread_end_time);

    printf("thread %u run time: %f seconds\n==========\n", 
            (unsigned int)self,
            difftime(thread_end_time, thread_start_time));

    return NULL;
}

/**
 * input:  root (the initial quadruple that defines the packing)
 *         limit (arbitrary limit we don't want to go above)
 * output: ancestors (list of quadruples all below the limit)
 */
void fuchsian(number root[4]) {
    unsigned char i = 0;
    number prime[4];
    number transformed[4];

    transformed[0] = 
            -3 * root[0] + ((root[0] + root[1] + root[2] + root[3]) << 1);
    transformed[1] = 
            -3 * root[1] + ((root[0] + root[1] + root[2] + root[3]) << 1);
    transformed[2] = 
            -3 * root[2] + ((root[0] + root[1] + root[2] + root[3]) << 1);
    transformed[3] = 
            -3 * root[3] + ((root[0] + root[1] + root[2] + root[3]) << 1);

    pthread_t fuchsianThreads[4];
    memset(fuchsianThreads, 0, sizeof(pthread_t) * 4);
    for (i = 0; i < 4; i++)
    {
#if DEBUGGING
        printf("transformed[%d] = %d\n", i, (int)transformed[i]);
        printf("root[%d]        = %d\n", i, (int)root[i]);
#endif
        if ( (root[i] < transformed[i]) 
             && (transformed[i] < ceiling))
        {
            memcpy(prime, root, 4*sizeof(number));
            prime[i] = transformed[i];
            if (setSetItem(CURVELIST, transformed[i], 1) < 0)
            {
                exit(1);
            }
            number *copy;
            // XXX: make sure copy is freed in fuchsianThreads!!!
            copy = (number *)malloc(4 * sizeof(number));
            memcpy(copy, prime, 4 * sizeof(number));

            if (pthread_create(fuchsianThreads + i, 
                               NULL, 
                               fuchsianThread, 
                               (void *)copy))
            {
                printf("Error creating fuchsian thread\n");
                exit(1);
            }
            else {
                printf("Created thread %u\n", 
                        (unsigned int)fuchsianThreads[i]);
                fflush(stdout);
            }
        }
    }

    //ll *curvatures[4]; // array of lla pointers
    //memset(curvatures, 0, sizeof(curvatures[0]) * 4);

    for (i = 0; i < 4; i++)
    {
        //ll *curvatures = NULL;
        int err = 0;
        if ((fuchsianThreads[i] != 0) 
            && (err = pthread_join(
                        (pthread_t)(fuchsianThreads[i]), 
                        NULL)))
                        //(void *)&curvatures)))
        {
            printf("Error joining fuchsian thread %d \n", 
                    (unsigned int)fuchsianThreads[i]);
            if (err == EDEADLK) { printf("EDEADLK\n"); }
            else if (err == EINVAL) { printf("EINVAL\n"); }
            else if (err == ESRCH) { printf("ESRCH\n"); }
            exit(1);
        }
        else
        {
            printf("Joined thread %u\n", (unsigned int)fuchsianThreads[i]);
            fflush(stdout);
            /*
            if (curvatures != NULL)
            {
                printf("curvatures address: %u\n", (unsigned int)curvatures);
                //llPrint(curvatures, 0);
                node *ptr = NULL;
                unumber len = curvatures->len;
                unumber count;
                for (count = 0; count < len; count++)
                {
                    ptr = llPop(curvatures);
                    //unsigned char curv;
                    //for (curv = 0; curv < 4; curv++)
                    //{
                        setSetItem(CURVELIST, ptr->val, 1);
                        //setAdd(CURVELIST, ptr->val[i]);
                    //}
                    nodeDestroy(ptr);
                }
                llDestroy(curvatures);
                curvatures = NULL;
            }
            else { printf("Curvatures was null\n"); }
            */
        }
    }

    setResetNumItems(CURVELIST);

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
struct LinkedListArray* transformOrbit(
    number quad[4], 
    struct LinkedListArray* orbit 
){
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
    memcpy(family[0], 
           ((number [4]){aPos,   b % 24, c % 24, d % 24}), 4*sizeof(number));
    memcpy(family[1], 
           ((number [4]){a % 24, bPos,   c % 24, d % 24}), 4*sizeof(number));
    memcpy(family[2], 
           ((number [4]){a % 24, b % 24, cPos,   d % 24}), 4*sizeof(number));
    memcpy(family[3], 
           ((number [4]){a % 24, b % 24, c % 24, dPos  }), 4*sizeof(number));

    unsigned char i;
    struct NodeArray *ptr = NULL;
    unsigned int matched = 0;
    int match = 1;
    for (i = 0; i < 4; i++)
    {
        ptr = NULL;
        matched = 0;
        match = 1;
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
    struct LinkedListArray *newGeneration = NULL;
    struct NodeArray *parent = NULL;
    for (parent = ancestors->header; parent != NULL; parent = parent->next)
    {
        newGeneration = transformOrbit(parent->val, orbit);
        llaExtend(ancestors, newGeneration);
        free(newGeneration);
        newGeneration = NULL;
    }
    llaDestroy(ancestors);

    return orbit;

}

set_t* path(set_t *valList) {

    set_t *could = setInitWithRange(LOW, ceiling);

    number i = 0;
    for (i = 0; i < ceiling; i++)
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
        // TODO: this could be made faster by using bitwise &
        if (setExists(valuesGlobal, i) && !setExists(CURVELIST, i))
        {
            node *n = (node *)nodeInitWithInt(i);
            llAppend(missing, n);
        }
    }

    return missing;

}

// TODO: use ceiling instead of cap
ll* seek(number root[4])
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
    pthread_mutex_init(&write_lock, NULL);
    fuchsian(root);
#if DEBUGGING
    printf("CURVELIST capacity:  %d\n", (int)setGetCapacity(CURVELIST));
    printf("CURVELIST num_items: "NUMFORM"\n", setGetNumItems(CURVELIST));
    setPrint(CURVELIST, 0);
#endif
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
    set_t *valuesGlobal = path(valuesOrbit);

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
        return 1;
    }

    time(&start_time);

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

    printf("==========\n");
    printf("Running seek with root {");
    for (index = 0; index < 4; index++) { printf(NUMFORM",",root[index]); }
    printf("} and ceiling %lld\n", ceiling);

    printTime();

    fflush(stdout);

    ll *results = seek(root);
    llPrint(results, 0);

    printTime();

    fflush(stdout);

    llDestroy(results);

    time(&end_time);
    printf("run time: %f seconds\n==========\n", difftime(end_time, start_time));
    
    return 0;
}  
