// Set implemented as a byte array

#ifndef SET_T_H_
#define

typedef char byte;

typedef struct set_s {
    byte *items;
    unsigned int num_items;
    unsigned int zero;
    unsigned int len;
} set_t;

// Initializers

set_t* setInit(void);

//set_t* setInitWithLen(unsigned int len);

//set_t* setInitWithZero(unsigned int zero);

set_t* setInitWithLenAndZero(unsigned int len, unsigned int zero);

set_t* setInitWithLenAndZeroAndItems(byte *items, unsigned int len, unsigned int zero);

// Setters and Getters

byte*         setGetItems(set_t *s);
void          setSetItems(set_t *s, byte *b);

unsigned int setGetNumItems(set_t *s);
void         setSetNumItems(set_t *s, unsigned int n);

unsigned int setGetLen(set_t *s);
void         setSetLen(set_t *s, unsigned int len);

unsigned int setGetZero(set_t *s);
void         setSetZero(set_t *s, unsigned int zero);

// Set Element Operations

unsigned int setAdd(set_t *s, unsigned int a);

unsigned int setRemove(set_t *s, unsigned int a);

char         setExists(set_t *s, unsigned int a);

#endif
