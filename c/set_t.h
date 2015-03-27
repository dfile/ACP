// Set implemented as a byte array

#ifndef SET_T_H_
#define SET_T_H_

typedef char byte;

typedef struct set_s {
    byte *items;
    unsigned int num_items;
    int zero;
    unsigned int len;
} set_t;

// Initializers

set_t* setInit(void);

// Use this initializer to make set_t's
set_t* setInitWithLenAndZero(unsigned int len, int zero);

// Calculates len and zero point from the range
set_t* setInitWithRange(int low, int high);

set_t* setInitWithLenAndZeroAndItems(unsigned int len, int zero, byte *items);

// Setters and Getters

byte*         setGetItems(set_t *s);
void          setSetItems(set_t *s, byte *b);

unsigned int setGetNumItems(set_t *s);
void         setSetNumItems(set_t *s, unsigned int n);

unsigned int setGetLen(set_t *s);
void         setSetLen(set_t *s, unsigned int len);

int          setGetZero(set_t *s);
void         setSetZero(set_t *s, int zero);

int          setGetLowRange(set_t *s);
int          setGetHighRange(set_t *s);

// Set Element Operations

int          setAdd(set_t *s, int a);

int          setRemove(set_t *s, int r);

byte         setExists(set_t *s, int e);

void         setPrint(set_t *s, byte opt);

void         setClear(set_t *s);

#endif
