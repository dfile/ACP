// C version of acp code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include "typedefs.h"
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "set_t.h"

typedef struct child_process_s
{
    pid_t pid;
    int pipes[2];
} child_process_t;

// the root quadruple, declared as a global variable to allow child
// processes to access it
number *ROOT = NULL;
number ROOTS[4][4];
unsigned char num_roots = 0;
unsigned char my_root;

// child's pipe
int *my_pipe;

// children
child_process_t children_fuchsian[4];

// the smallest curvature in root quad, used to determine set_t range
number LOW;  

set_t *CURVELIST;

long long int ceiling;

// file descriptors for pipes between processes
const int PIPE_W = 1;     // aliases
const int PIPE_R = 0;

time_t t;
time_t start_time;
time_t end_time;
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
void fuchsian(number root[4]) 
{
    struct LinkedListArray ancestors;
    memset(&ancestors, 0, sizeof(lla));

    llaAppend(&ancestors, nodeArrayInitWithArray(root));
    struct NodeArray *n = NULL;
    unsigned char i;
    // add root to CURVELIST
    // this is necessary b/c of how the children are made
    for (i = 0; i < 4; i++)
    {
        if (setAdd(CURVELIST, root[i]) < 0)
        { exit(1); }
    }
    number prime[4];
    number transformed[4];

    while (ancestors.len > 0)
    {
        n = llaPop(&ancestors);

        transformed[0] = -3 * n->val[0] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[1] = -3 * n->val[1] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[2] = -3 * n->val[2] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);
        transformed[3] = -3 * n->val[3] + ((n->val[0] + n->val[1] + n->val[2] + n->val[3]) << 1);

        for (i = 0; i < 4; i++)
        {
            if ( (n->val[i] < transformed[i]) 
                 && (transformed[i] < ceiling))
            {
                memcpy(prime, n->val, 4*sizeof(number));
                prime[i] = transformed[i];
                llaPush(&ancestors, nodeArrayInitWithArray(prime));
                if (setSetItem(CURVELIST, transformed[i], 1) < 0)
                { exit(1); }
            }
        }
        nodeArrayDestroy(n);
        n = NULL;
    }

    unumber capacity = setGetCapacity(CURVELIST);
    unumber index = 0;
    for (index = 0; index < capacity; index++)
    {
        write(my_pipe[PIPE_W], &(CURVELIST->items[index]), sizeof(byte));
    }

    return;
}

void fuchsian_divide(number root[4])
{
    memset(ROOTS, 0, sizeof(ROOTS));
    number transformed[4];
    transformed[0] = -3 * root[0] + ((root[0] + root[1] + root[2] + root[3]) << 1);
    transformed[1] = -3 * root[1] + ((root[0] + root[1] + root[2] + root[3]) << 1);
    transformed[2] = -3 * root[2] + ((root[0] + root[1] + root[2] + root[3]) << 1);
    transformed[3] = -3 * root[3] + ((root[0] + root[1] + root[2] + root[3]) << 1);

    unsigned char i = 0;
    for (i = 0; i < 4; i++)
    {
        number prime[4];
        memset(prime, 0, sizeof(prime[0]) * 4);
        if ( (root[i] < transformed[i]) 
                && (transformed[i] < ceiling))
        {
            memcpy(prime, root, 4*sizeof(number));
            prime[i] = transformed[i];
            memcpy(ROOTS[num_roots], prime, sizeof(prime[0]) * 4);
            ++num_roots;
        }
    }
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

set_t* valuesOf(struct LinkedListArray* quadList) {
    set_t *possible = setInitWithRange(0, 24);
    struct NodeArray *ruple = NULL;
    for (ruple = quadList->header; ruple != NULL; ruple = ruple->next)
    {
        unsigned char i;
        for (i = 0; i < 4; i++)
        {
            if (setAdd(possible, ruple->val[i]) < 0)
            { exit(1); }
        }
    }

    return possible;
}

ll* pathAndCompare(set_t *valList) {

    ll *missing = (ll *)llaInit();
    number i = 0;
    number posI = 0;
    for (i = LOW; i < ceiling; i++)
    {
        if ((i % 24) < 0) { posI = i + 24; }
        else              { posI = i;      }
        if (setExists(valList, (posI % 24)) && !setExists(CURVELIST, i))
        {
            node *n = (node *)nodeInitWithInt(i);
            llAppend(missing, n);
        }
    }

    return missing;
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

void child_fuchsian(const unsigned char child)
{
    CURVELIST = setInitWithRange(LOW, ceiling);
    my_pipe = children_fuchsian[child].pipes;
    printf("I'm child %u\n", (unsigned int)child);
    close(my_pipe[PIPE_R]);    // child is writing only
    printf("Running fuchsian\n");
    fflush(stdout);
    fuchsian(ROOTS[my_root]);
    printf("exiting child %u\n", (unsigned int)child);
    fflush(stdout);
    setDestroy(CURVELIST);
    exit(0);
}

void receive_fuchsian_curvatures(const int max_pipe_num)
{
    byte aByte = 0;
    unumber byte_num = 0;
    int pipe_num = 0;
    int ret_val;
    while (pipe_num < max_pipe_num)
    {
        if (byte_num < CURVELIST->capacity)
        {
            ret_val = read(children_fuchsian[pipe_num].pipes[PIPE_R], 
                           &aByte, 
                           sizeof(aByte));
        }
        else
        { ret_val = -1; }
        if (ret_val != -1) 
        { 
            CURVELIST->items[byte_num] = CURVELIST->items[byte_num] | aByte;
            ++byte_num;
        }
        else
        { 
            ++pipe_num; 
            aByte = 0;
            byte_num = 0;
            printf("one child finished, waiting on it\n");
            fflush(stdout);
            int status;
            waitpid(-1, &status, 0);
        }
    } // end while
} // end receive_fuchsian_curvatures

ll* seek(number root[4])
{

    printf("In seek\n");


    fuchsian_divide(root);
    memset(children_fuchsian, 0, sizeof(children_fuchsian[0]) * 4);
    unsigned char num_children = 0;
    // make the children for fuchsian
    for (num_children = 0; num_children < num_roots; num_children++)
    {
        pipe(&(children_fuchsian[num_children].pipes[0])); 
        my_root = num_children; 
        printf("making child %u...\n", (unsigned int)num_children);
        fflush(stdout);
        if ((children_fuchsian[num_children].pid = fork()) == -1)
        {
            fprintf(STDERROR, "error making fork\n");
            exit(1);
        }
        else if (children_fuchsian[num_children].pid == 0)     
        { child_fuchsian(num_children); } // child's start of execution
        // parent is reading only
        close(children_fuchsian[num_children].pipes[PIPE_W]);    
    }

    // Only parent gets past the for loop
    // Receive fuchsian results from children and put them in CURVELIST
    CURVELIST = setInitWithRange(LOW, ceiling);
    unsigned char i = 0;
    for (i = 0; i < 4; i++)
    {
        if (setAdd(CURVELIST, root[i]) < 0)
        { exit(1); }
    } 
    receive_fuchsian_curvatures(num_children);

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
    llaPrint(admissible);

    printTime();
    printf("Running valuesOf\n");
    fflush(stdout);
    set_t *valuesOrbit = valuesOf(admissible);

    llaDestroy(admissible);

    printTime();

    printf("Running path and compare\n");
    fflush(stdout);
    ll *nope = pathAndCompare(valuesOrbit);

    setDestroy(CURVELIST);

    return nope;
}

int main(int argc, char *argv[]) {
    
    time(&start_time);


    if (argc != 6)
    {
        fprintf(stderr, "ERR: Incorrect number of arguments.\n");
        fprintf(stderr, "Usage: acp curv1 curv2 curv3 curv4 ceiling\n");
        fprintf(stderr, "Example: acp -1 2 2 3 1000\n");
        return 1;
    }

    number root[4] = {0,0,0,0};

    unsigned char arg = 1;
    for (arg = 1; arg < 5; arg++)
    {
        root[arg-1] = (number)strtol(argv[arg], NULL, 10);
    }
    ROOT = root;

    number index = 0;
    LOW = ROOT[0];
    for (index = 0; index < 4; index++)
    {
        if (ROOT[index] < LOW) { LOW = ROOT[index]; }
    }

    ceiling = (long long int)strtoll(argv[5], NULL, 10);

    printf("==========\n");
    printf("Running seek with root {");
    for (index = 0; index < 4; index++) { printf(NUMFORM",",ROOT[index]); }
    printf("} and ceiling %lld\n", ceiling);

    printTime();

    fflush(stdout);

    ll *results = seek(ROOT);
    llPrint(results, 0);

    printTime();

    fflush(stdout);

    llDestroy(results);

    time(&end_time);
    printf("run time: %f\n==========\n", difftime(end_time, start_time));
    
    return 0;
}  
