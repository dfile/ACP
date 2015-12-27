// Set implemented as a bit array

#ifndef SET_T_H_
#define SET_T_H_

#include "typedefs.h"

typedef uint8_t set_t_unit;

typedef struct set_s {
    set_t_unit *items;  // items of the set
    unumber num_items;  // number of items set to 1 in set
    number zero;        // offset low range to 0 index
    unumber len;        // array length of items
    unumber capacity;   // how many numbers can be held in items
} set_t;

// Initializers

 set_t* setInit(void);

 // Use this initializer to make set_t's
 set_t* setInitWithLenAndZero(unumber len, number zero);

 // Calculates len and zero point from the range
 set_t* setInitWithRange(number low, number high);

 set_t* setInitWithLenAndZeroAndItems(unumber len, number zero, set_t_unit *items);

// Uninitializers

 void setDestroy(set_t *s);

// Convenience Functions

 unumber setOffsetToZero(set_t *s, number n);

// Setters and Getters

 set_t_unit   setSetItem(set_t *s, number n, set_t_unit b);

 set_t_unit*  setGetItems(set_t *s);
 void         setSetItems(set_t *s, set_t_unit *b);

 unumber      setGetNumItems(set_t *s);
 void         setSetNumItems(set_t *s, unumber n);
 void         setResetNumItems(set_t *s);

 unumber      setGetLen(set_t *s);
 void         setSetLen(set_t *s, unumber len);

 number       setGetZero(set_t *s);
 void         setSetZero(set_t *s, number zero);

 unumber      setGetCapacity(set_t *s);
 void         setSetCapacity(set_t *s, unumber capacity);
 void         setSetCapacityWithLen(set_t *s, unumber len);

 number       setGetLowRange(set_t *s);
 number       setGetHighRange(set_t *s);

// Set Element Operations

 number       setAdd(set_t *s, number a);

 number       setRemove(set_t *s, number r);

 set_t_unit   setExists(set_t *s, number e);

 void         setPrint(set_t *s, set_t_unit opt);

 void         setClear(set_t *s);

#endif
