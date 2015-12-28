// C version of perhaps code
// This version is incomplete. It was originally coded to use arrays,
// but linked lists will be used instead.

#include <stdio.h>
#include <stdbool.h>

// input: quad (a quadruple of curvatures)
// output: solutions (list of transformed quadruples)
// function: this runs quadruples through the four transformations (one for each curvature), and records the new one only if the transformed curvature is bigger than the original (so a smaller circle in the packing)
int * transform(int quad[4]) {
    
    int solutions[4][4] = {NULL};
    int a = quad[0];
    int b = quad[1];
    int c = quad[2];
    int d = quad[3];
    
    if ((-a + (2 * (b + c + d))) > a) {
        solutions[0] = {(-a + (2 * (b + c + d))), b, c, d};
    }
    if ((-b + (2 * (a + c + d))) > b) {
        solutions[1] = {a, (-b + (2 * (a + c + d))), c, d};
    }
    if ((-c + (2 * (a + b + d))) > c) {
        solutions[2] = {a, b, (-c + (2 * (a + b + d))), d};
    }
    if ((-d + (2 * (a + b + c))) > d) {
        solutions[3] = {a, b, c, (-d + (2 * (a + b + c)))};
    }
    
    return solutions;
}

/**
* input:  quadList (a list of quadruples)
*         ceiling (the arbitrary limit we don't want to go above)
* output: validQuads (list of quadruples where every curv is below the ceiling)
*/
int * check(int * quadlist, int ceiling) {
    int validQuads[100][4] = {NULL};   // TODO: 100 is arbitrary, find the real value
    int  i,j;             // iterator vars
    int  row = 0;         // incremented inside for loop
    int  col = 4;         // iterator var
    bool valid;           // boolean flag

    // TODO: row can't be 0 when entering the for loop
    for (i = 0; i < row; i++) {     // for quad in quadList
        valid = true;
        // TODO: this logic isn't right. I think i shouldn't be decremented, and instead row is somehow manipulated.
        if (quadList[i] == NULL) {
            i--;            // Even though this index is null,
            continue;       // we don't want to skip a spot in
        }                   // quadlist, so decrement i
        for (j = 0; j < col; j++) { //    for curv in quad
            if (entry >= ceiling) {
                valid = false;
            }
        }
        if (valid) {
            validQuads[row] = quadlist[i];
            row++;
        }
    }
    return validQuads;
}

/**
* input:  root (the initial quadruple that defines the packing)
*         limit (arbitrary limit we don't want to go above)
* output: ancestors (list of quadruples all below the limit)
*/
int * fuchsian(int root[4], int limit) {

    int ancestors[100][4] = {NULL};  // TODO: 100 is arbitrary, figure out a better size
    ancestors[0] = root;
    int i,j;                // iterator vars
    int row = 1;
    int col = 4
    int nextGen[4][4] = {NULL};
    int validGen[100][4] = {NULL};
    int validGenSize = sizeof(validGen) / sizeof(validGen[0]);
    
    for (i = 0; i < row; i++) {
        nextGen = transform(ancestors[i]);
        validGen = check(nextGen, limit);
        
        j = 0;
        //// TODO: Sharif's Bookmark
        while(validGen[j] != NULL) {
            ancestors[i] = validGen[j];
            j++;
            row++;
        }
    }

    return ancestors;
}
