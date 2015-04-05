// Set implemented as a byte array

#ifndef SET_T_H_
#define SET_T_H_

#include "typedefs.h"

typedef char byte;

typedef struct set_s {
    byte *items;
    unumber num_items;
    number zero;
    unumber len;
} set_t;

// Initializers

 set_t* setInit(void);

 // Use this initializer to make set_t's
 set_t* setInitWithLenAndZero(unumber len, number zero);

 // Calculates len and zero point from the range
 set_t* setInitWithRange(number low, number high);

 set_t* setInitWithLenAndZeroAndItems(unumber len, number zero, byte *items);

// Uninitializers

 void setDestroy(set_t *s);

// Setters and Getters

 byte*         setGetItems(set_t *s);
 void          setSetItems(set_t *s, byte *b);

 unumber setGetNumItems(set_t *s);
 void         setSetNumItems(set_t *s, unumber n);

 unumber setGetLen(set_t *s);
 void         setSetLen(set_t *s, unumber len);

 number          setGetZero(set_t *s);
 void         setSetZero(set_t *s, number zero);

 number          setGetLowRange(set_t *s);
 number          setGetHighRange(set_t *s);

// Set Element Operations

 number          setAdd(set_t *s, number a);

 number          setRemove(set_t *s, number r);

 byte         setExists(set_t *s, number e);

 void         setPrint(set_t *s, byte opt);

 void         setClear(set_t *s);

#endif
