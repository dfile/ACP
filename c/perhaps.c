// C version of perhaps code

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedListArray.h"
#include "LinkedList.h"
#include "hashsetint.h"

// input: quad (a quadruple of curvatures)
// output: solutions (list of transformed quadruples)
// function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
struct LinkedListArray* transform(int quad[4]) {

    struct LinkedListArray *solutions = llaInit();
    int a = quad[0];
    int b = quad[1];
    int c = quad[2];
    int d = quad[3];
    struct NodeArray *soln = NULL;
    if ((-a + (2 * (b + c + d))) > a) {
        soln = nodeArrayInit();
        memcpy((soln->val), 
                ((int [4]){(-a + (2 * (b + c + d))), b, c, d}),
                (4*sizeof(int)));
        llaAppend(solutions, soln);
    }
    if ((-b + (2 * (a + c + d))) > b) {
        soln = nodeArrayInit();
        memcpy((soln->val), 
               ((int [4]){a, (-b + (2 * (a + c + d))), c, d}),
               (4*sizeof(int)));
        llaAppend(solutions, soln);
    }
    if ((-c + (2 * (a + b + d))) > c) {
        soln = nodeArrayInit();
        memcpy((soln->val), 
                ((int [4]){a, b, (-c + (2 * (a + b + d))), d}),
                (4*sizeof(int)));
        llaAppend(solutions, soln);
    }
    if ((-d + (2 * (a + b + c))) > d) {
        soln = nodeArrayInit();
        memcpy((soln->val), 
                ((int [4]){a, b, c, (-d + (2 * (a + b + c)))}),
                (4*sizeof(int)));
        llaAppend(solutions, soln);
    }

    return solutions;
}

/**
 * input:  quadList (a list of quadruples)
 *         ceiling (the arbitrary limit we don't want to go above)
 * output: validQuads (list of quadruples where every curv is below the ceiling)
 */
struct LinkedListArray* check(struct LinkedListArray *quadList, int ceiling) {

    struct LinkedListArray *validQuads = llaInit();
    struct NodeArray *validQuad = NULL;
    struct NodeArray *quadrupleNode = NULL;
    int entry;
    int quadruple[4];
    bool valid = true;

    for (quadrupleNode = quadList->header; quadrupleNode != NULL; quadrupleNode = quadrupleNode->next)
    {
        valid = true;

        // Copy array in quadrupleNode to quadruple
        memcpy(quadruple, 
                quadrupleNode->val,
                4*sizeof(int));
        for (entry = 0; entry < 4; entry++)
        {
            if (quadruple[entry] >=ceiling)
            {
                valid = false;
            }
        }
        if (valid) {
            validQuad = nodeArrayInit();

            // Copy quadruple array to validQuad node
            memcpy(validQuad->val, 
                    quadruple,
                    4*sizeof(int));
            llaAppend(validQuads, validQuad);
        }
    }

    return validQuads;
}

/**
 * input:  root (the initial quadruple that defines the packing)
 *         limit (arbitrary limit we don't want to go above)
 * output: ancestors (list of quadruples all below the limit)
 */
struct LinkedListArray* fuchsian(int root[4], int limit) {

    struct LinkedListArray *ancestors = llaInit();
    struct NodeArray *rootNode = nodeArrayInit();
    memcpy(rootNode->val,
            root,
            4*sizeof(int));
    llaAppend(ancestors, rootNode);

    struct NodeArray *parent = NULL;
    struct NodeArray *child = NULL;
    for (parent = ancestors->header; parent != NULL; parent = parent->next)
    {
        int parentArray[4];
        memcpy(parentArray,
                parent->val,
                4*sizeof(int));
        struct LinkedListArray *nextGen = transform(parentArray);
        struct LinkedListArray *validGen = check(nextGen, limit);
        for (child = validGen->header; child != NULL; child = child->next)
        {
            struct NodeArray *childCopy = nodeArrayInit();
            memcpy(childCopy->val,
                    child->val,
                    sizeof(child->val));
            llaAppend(ancestors, childCopy);
        }
        free(nextGen);
        free(validGen);
    }

    return ancestors;
}

struct LinkedList* valuesOf(struct LinkedListArray* quadList) {

    struct NodeArray *ruple = NULL;
    hashset_t maybe = hashset_create();
    struct LinkedList* actual = llInit();
    int i;
    for (ruple = quadList->header; ruple != NULL; ruple = ruple->next)
    {
        for (i = 0; i < 4; i++)
        {
            // Hashset never recognizes equal vals because it's comparing pointers, not values.
            // Might need to implement a binary search tree instead. For now just do a SLOW search
            // using linked lists.
            int exists = hashset_add(maybe, (ruple->val[i]));
            if (exists == 1)
            {
                struct Node *node = nodeInit();
                node->val = ruple->val[i];
                llAppend(actual, node);
            }
        }
    }

    return actual;
}

// Only returns solutions, not an updated orbit because orbit is actually modified in this function since it's a pointer
struct LinkedListArray* transformOrbit(int quad[4], struct LinkedListArray* orbit) {
    
    //printf(">>transformOrbit<<\n\n");
    struct LinkedListArray *solutions = llaInit();
    int a = quad[0];
    int b = quad[1];
    int c = quad[2];
    int d = quad[3];
    int family[4][4];
    unsigned int i;
    struct NodeArray *ptr = NULL;
    int match = 1;

    int aPos = (-a + (2 * (b + c + d))) % 24;
    int bPos = (-b + (2 * (a + c + d))) % 24;
    int cPos = (-c + (2 * (a + b + d))) % 24;
    int dPos = (-d + (2 * (a + b + c))) % 24;

    aPos = aPos < 0 ? aPos + 24 : aPos;
    bPos = bPos < 0 ? bPos + 24 : bPos;
    cPos = cPos < 0 ? cPos + 24 : cPos;
    dPos = dPos < 0 ? dPos + 24 : dPos;

    // the four matrices
    int aPrime[4] = {aPos, c % 24, b % 24, d % 24};
    int bPrime[4] = {a % 24, bPos, c % 24, d % 24};
    int cPrime[4] = {a % 24, b % 24, cPos, d % 24};
    int dPrime[4] = {a % 24, b % 24, c % 24, dPos};



    memcpy(family[0],
            aPrime,
            sizeof(aPrime));
    memcpy(family[1],
            bPrime,
            sizeof(bPrime));
    memcpy(family[2],
            cPrime,
            sizeof(cPrime));
    memcpy(family[3],
            dPrime,
            sizeof(dPrime));

    for (i = 0; i < 4; i++)
    {
        for (ptr = orbit->header; ptr != NULL; ptr = ptr->next)
        {
            // If memcmp finds family[i] in orbit, match will be 0, so
            // multiplying by 0 will always give 0, and match will end
            // up as 0.
            int j = 0;
            //printf("Comparing ["); for (j = 0; j < 4; j++) { printf("%d,", family[i][j]); } printf("] ");
            //printf("against "); nodeArrayPrint(ptr);

            match = match * memcmp(family[i], ptr->val, 4*sizeof(int));

            //printf(" %d\n", match);
        }
        // If family[i] was not in orbit, append it to orbit
        if (match != 0)
        {
            //printf("Appending array to orbit\n");
            struct NodeArray *kid = nodeArrayInit();
            memcpy(kid->val,
                    family[i],
                    sizeof(family[i])); 
            llaAppend(orbit, kid);

            struct NodeArray *kid1 = nodeArrayInit();
            memcpy(kid1,
                    kid,
                    sizeof(struct NodeArray));
            kid1->next = NULL;
            kid1->prev = NULL;

            llaAppend(solutions, kid1);
        }
        /*
        else
        {
            int j = 0;
            //printf("This array: [");
            for (j = 0; j < 4; j++)
            {
                //printf("%d,", family[i][j]);
            }
            //printf("] ");
            //printf(" matched one or more array in orbit.");
            //llaPrint(orbit);
        }
        */
        match = 1;
    }

    //printf(">>End transformOrbit<<\n\n");
    return solutions;

}

struct LinkedListArray* genealogy(int seed[4]) {

    struct LinkedListArray *ancestors = llaInit();
    struct LinkedListArray *orbit = llaInit();
    int modSeed[4] = {0};
    unsigned int i;
    struct NodeArray *parent = NULL;
    struct NodeArray *offspring = NULL;

    for (i = 0; i < 4; i++)
    {
        //printf("seed[i] = %d\n", seed[i]);
        if ((modSeed[i] = (seed[i] % 24)) < 0)
        {
            modSeed[i] = modSeed[i] + 24;
        }
        //printf("modSeed[i] = %d\n", modSeed[i]);

    }

    struct NodeArray *modSeedNode = nodeArrayInit();
    memcpy(modSeedNode->val,
            modSeed,
            sizeof(modSeed));
    llaAppend(ancestors, modSeedNode);

    struct NodeArray *modSeedNode1 = nodeArrayInit();
    memcpy(modSeedNode1,
            modSeedNode,
            sizeof(struct NodeArray));
    //printf("original orbit pre:  ");    llaPrint(orbit); 
    llaAppend(orbit, modSeedNode);
    //printf("original orbit post: ");    llaPrint(orbit);
    //int cnt = 0;
    for (parent = ancestors->header; parent != NULL; parent = parent->next)
    {
        //printf("%d: orbit pre:  ", cnt);  llaPrint(orbit);
        struct LinkedListArray *newGeneration = transformOrbit(parent->val, orbit);
        //printf("%d: orbit post: ", cnt);  llaPrint(orbit);
        //if (++cnt < 3) { printf("Exiting\n"); break; }
        //printf("newGeneration: ");  llaPrint(newGeneration);
        for (offspring = newGeneration->header; offspring != NULL; offspring = offspring->next)
        {
            struct NodeArray *offspringCopy = nodeArrayInit();
            memcpy(offspringCopy,
                    offspring,
                    sizeof(struct NodeArray));
            llaAppend(ancestors, offspringCopy);
        }
    }

    return orbit;

}

struct LinkedList* path(struct LinkedList *valList, int top) {

    struct LinkedList *could = llInit();
    int i = 0;
    struct Node *poss = NULL;

    for (i = 0; i < top; i++)
    {
        for (poss = valList->header; poss != NULL; poss = poss->next)
        {
            //printf("Comparing %d (%d) and %d\n", i, i % 24, poss->val);
            if ((i % 24) == poss->val)
            {
                //printf("added\n");
                struct Node *possCopy = nodeInit();
                possCopy->val = i;
                llAppend(could, possCopy);
                //hashset_add(could, i);
                break;
            }
        }
    }

    return could;

}

struct LinkedList* compare(struct LinkedList *valsPack, struct LinkedList *valsOrb, int limit) {

    struct LinkedList *missing = llInit();
    printf("About to call path. valsOrb: ");
    llPrint(valsOrb);
    struct LinkedList *should = path(valsOrb, limit);
    printf("Path results: "); llPrint(should);
    hashset_t valsPackSet = hashset_create();
    struct Node *might = NULL;
    struct Node *ptr = NULL;

    for (ptr = valsPack->header; ptr != NULL; ptr = ptr->next)
    {
        hashset_add(valsPackSet, (ptr->val));
    }

    for (might = should->header; might != NULL; might = might->next)
    {
        if (hashset_is_member(valsPackSet, (might->val)) == 0)
        {
            struct Node *node = nodeInit();
            node->val = might->val;
            llAppend(missing, node);
        }
    }

    return missing;

}

struct LinkedList* seek(int root[4], int cap)
{

    printf("In seek\n");
    struct LinkedListArray *small = fuchsian(root, cap);
    printf("  fuchsian results - matches with acp python code \n");
    //llaPrint(small);
    struct LinkedList *valuesPack = valuesOf(small);
    printf("  valuesOf results from fuchsian - matches with acp python code \n");
    //llPrint(valuesPack);
    struct LinkedListArray *admissible = genealogy(root);
    printf("  genealogy results - matches with acp python code\n");
    //llaPrint(admissible);
    struct LinkedList *valuesOrbit = valuesOf(admissible);
    printf("  valuesOf results from genealogy - mathces with acp python code\n");
    //llPrint(valuesOrbit);
    struct LinkedList *gone = compare(valuesPack, valuesOrbit, cap);

    return gone;
}

void main(void) {

    printf("Running perhaps...\n");

    int root[4] = {-1, 2, 2, 3};
    struct LinkedList *results = seek(root, 1000);
    llPrint(results);

}  
