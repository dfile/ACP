// Implementation of set_t

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "set_t.h"

// Bare-bones initializer, just allocates memory and returns
set_t* setInit(void)
{
    return setInitWithLenAndZeroAndItems(0, 0, NULL);
}

// This is the initializer that should be used when making sets
set_t* setInitWithLenAndZero(unumber len, number zero)
{
    //printf("malloc set items: "NUMFORM" bytes\n", sizeof(byte) * len);
    unumber cap = ((len - 1) / 8) + 1;
    byte *b = (byte *)malloc(sizeof(byte) * (cap));
    if (b == NULL) 
    {
        fprintf(stderr, "ERR: Couldn't malloc in setInitWithLenAndZero()\n");
        exit(1);
    }
    else
    {
        memset(b, 0, cap);
    }
    set_t *s = setInitWithLenAndZeroAndItems(len, zero, b);
    return s;
}

// Use this initializer too
set_t* setInitWithRange(number low, number high)
{
    unumber len = high - low + 1;
    number zero = 0 - low;
    return setInitWithLenAndZero(len, zero);
}

// Designated initializer
set_t* setInitWithLenAndZeroAndItems(unumber len, number zero, byte *items)
{
    //printf("malloc set struct: %d bytes\n", sizeof(struct set_s));
    set_t *set = malloc(sizeof(struct set_s));
    setSetItems(set, items);
    setSetNumItems(set, 0);
    setSetLen(set, len);
    setSetZero(set, zero);
    setSetCapacityWithLen(set, len);
    return set;
}

// Uninitializer
void setDestroy(set_t *s)
{
    if (s == NULL) { return; }
    free(s->items);
    //printf("destroy set items\n");
    free(s);
    //printf("destroy set struct\n");
    s = NULL;
}

inline unumber setOffsetToZero(set_t *s, number n)
{
    return n + setGetZero(s);
}

inline number setOffsetToNormal(set_t *s, unumber u)
{
    return u - setGetZero(s);
}

inline byte setIsInRange(set_t *s, number n)
{
    n = setOffsetToZero(s, n);
    return (n >= 0 && n < setGetLen(s));
}

inline byte setSetItem(set_t *s, number n, byte b)
{
    if (s != NULL)
    {
        if (s->items != NULL)
        {
            if (setIsInRange(s, n))
            {
                byte mask = b ? b : 1;
                unumber index = setOffsetToZero(s, n);
                mask = mask << (index % 8);
                if (b != 0)
                {
                    s->items[index / 8] = s->items[index / 8] | mask;
                }
                else
                {
                    s->items[index / 8] = s->items[index / 8] & ~mask;
                }
                return 0;
            }
            else
            {
                fprintf(stderr, "ERR: "NUMFORM" is out of range of set in setSetItem()\n", n);
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "ERR: items of set is NULL in setSetItem()\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERR: set is NULL in setSetItem()\n");
        exit(1);
    }
}

byte* setGetItems(set_t *s)
{
    if (s != NULL) { return s->items; }
    else
    {
        fprintf(stderr, "ERR: set_t NULL in setGetItems()\n");
        exit(1);
        return NULL;
    }
}

void setSetItems(set_t *s, byte *b)
{
    if (s != NULL) { 
        if (b == NULL) { 
            fprintf(stderr, "ERR: Trying to set NULL items to set in setSetItems()\n");
            exit(-1); 
        }
        s->items = b;
    }
    else
    {
        fprintf(stderr, "ERR: Trying to set items of NULL set in setSetItems()\n");
        exit(1);
    }
}

unumber setGetNumItems(set_t *s)
{
    if (s != NULL) { return s->num_items; }
    else
    {
        fprintf(stdout, "set_t NULL in setGetNumItems()");
        return 0;
    }
}

void setSetNumItems(set_t *s, unumber n)
{
    if (s != NULL) { s->num_items = n; }
    else
    {
        fprintf(stderr, "Trying to set num_items of NULL set in setSetNumItems()\n");
        exit(1);
    }
}

void setResetNumItems(set_t *s)
{
    if (s != NULL)
    {
        unumber i = 0;
        unumber c = 0;
        //unumber j = 0;
        byte *items = setGetItems(s);
        for (i = 0; i < setGetCapacity(s); i++)
        {
            byte b = items[i];
            for (; b; b >>= 1)
            {
                c += b & 1;
                //printf("%d\n", j++);
            }
        }
        setSetNumItems(s, c);
    }
    else
    {
        fprintf(stderr, "Trying to reset num_items of NULL set in setSetNumItems()\n");
        exit(1);
    }
}

unumber setGetLen(set_t *s)
{
    if (s != NULL) { return s->len; }
    else
    {
        fprintf(stdout, "set_t NULL in setGetLen()\n");
        return 0;
    }
}

void setSetLen(set_t *s, unumber len)
{
    if (s != NULL) { s->len = len; }
    else
    {
        fprintf(stderr, "Trying to set len of NULL set in setSetLen()\n");
        exit(1);
    }
}

number setGetZero(set_t *s)
{
    if (s != NULL) { return s->zero; }
    else
    {
        fprintf(stdout, "set_t NULL in setGetZero()\n");
        return 0;
    }
}

void setSetZero(set_t *s, number zero)
{
    if (s != NULL)
    {
        s->zero = zero;
    }
    else
    {
        fprintf(stderr, "Trying to set zero of NULL set in setSetZero()\n");
        exit(1);
        return;
    }
}

unumber setGetCapacity(set_t *s)
{
    if (s != NULL) { return s->capacity; }
    else
    {
        fprintf(stdout, "set_t NULL in setGetCapacity()\n");
        return 0;
    }
}

void setSetCapacity(set_t *s, unumber capacity)
{
    if (s != NULL)
    {
        s->capacity = capacity;
    }
    else
    {
        fprintf(stderr, "Trying to set capacity of NULL set in setSetCapacity()\n");
        exit(1);
        return;
    }
}

void setSetCapacityWithLen(set_t *s, unumber len)
{
    if (s != NULL)
    {
        s->capacity = ((len - 1) / 8) + 1;
    }
    else
    {
        fprintf(stderr, "Trying to set capacity of NULL set in setSetCapacity()\n");
        exit(1);
        return;
    }
}

inline number setGetLowRange(set_t *s)
{
    return (0 - setGetZero(s));
}

inline number setGetHighRange(set_t *s)
{
    return (setGetLen(s) - 1 - setGetZero(s));
}

inline void setGetRange(set_t *s, char *r)
{
    //char *r = (char *)malloc(sizeof(char) * 30);
    sprintf(r, "["NUMFORM" to "NUMFORM"]", setGetLowRange(s), setGetHighRange(s));
}

inline byte setExists(set_t *s, number e)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            if (setIsInRange(s, e))
            {
                
                unumber index = setOffsetToZero(s, e);
                unumber i = index / 8;
                byte pos = index % 8;
                byte mask = 1 << pos;
                
                return (setGetItems(s)[i] & mask);
            }
            else
            {
                char range[50];
                setGetRange(s, range);
                fprintf(stderr, "ERR: "NUMFORM" is outside of bounds %s of byte array of set in setExists()\n", e, range);
                //free(range);
                return 0;
            }
        }
        else
        {
            fprintf(stderr, "ERR: Can't check "NUMFORM" in NULL byte array of set in setExists()\n", e);
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "ERR: Can't check "NUMFORM" in NULL set in setExists()\n", e);
        return 0;
    }
    return (byte)(0);

}

number setAdd(set_t *s, number a)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            if (setIsInRange(s, a))
            {
                // increment num_items only if index doesn't exist (is 0)
                //printf("Adding "NUMFORM" at "NUMFORM" to set\n", a, index);
                setSetNumItems(s, (setExists(s, a) ? setGetNumItems(s) : (setGetNumItems(s) + 1)) );
                setSetItem(s, a, (byte)1);
                return 0;
            }
            else
            {
                char range[50];
                setGetRange(s, range);
                fprintf(stderr, "ERR: "NUMFORM" is outside of bounds %s of byte array of set in setAdd()\n", a, range);
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "ERR: Trying to add "NUMFORM" to NULL byte array of set in setAdd()\n", a);
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "ERR: Trying to add "NUMFORM" to NULL set in setAdd()\n", a);
        return -1;
    }
}

number setRemove(set_t *s, number r)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            if (setIsInRange(s, r))
            {
                // decrement num_items only if index exists (is 1)
                //printf("Removing "NUMFORM" at "NUMFORM" from set\n", r, setOffsetToZero(s, r));
                setSetNumItems(s, (setExists(s, r) ? setGetNumItems(s) - 1 : setGetNumItems(s)) );
                setSetItem(s, r, (byte)0);
                return 0;
            }
            else
            {
                char range[50];
                setGetRange(s, range);
                fprintf(stderr, "ERR: "NUMFORM" is outside of bounds %s of byte array of set in setRemove()\n", r, range);
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "ERR: Can't remove "NUMFORM" from NULL byte array of set in setRemove()\n", r);
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "ERR: Can't remove "NUMFORM" from NULL set in setRemove()\n", r);
        return -1;
    }
    return -1;
}


void setPrintDefault(set_t *s)
{
    printf("Set items: [");
    unumber i = 0;
    for (i = 0; i < setGetLen(s); i++)
    {
        if (setExists(s, setOffsetToNormal(s, i)))
        {
            if (i + 1 == setGetLen(s))
            {
                printf(""NUMFORM"", i - setGetZero(s));
            }
            else
            {
                printf(""NUMFORM",", i - setGetZero(s));
            }
        }
    }
    printf("]\n");
}

void setPrintByteArray(set_t *s)
{
    printf("Set items: [\n");
    unumber i = 0;
    for (i = 0; i < setGetLen(s); i++)
    {
        printf(NUMFORM": "NUMFORM"\n", setOffsetToNormal(s, i), (number)(setExists(s, setOffsetToNormal(s, i))));
    }
    printf("]\n");
}

void setPrintVerbose(set_t *s)
{
    setPrintDefault(s);
    setPrintByteArray(s);
}

void setPrint(set_t *s, byte opt)
{
    if (s == NULL) { printf("Set is NULL\n"); }
    else
    {
        printf("Set num_items: "UNUMFORM"\n", setGetNumItems(s));
        printf("Set zero: "NUMFORM"\n", setGetZero(s));
        printf("Set len: "UNUMFORM"\n", setGetLen(s));
        printf("Set capacity: "UNUMFORM"\n", setGetCapacity(s));
        char range[50];
        setGetRange(s, range);
        printf("Set range: %s\n", range);

        if (setGetItems(s) == NULL) { printf("Set items: NULL\n"); }
        else
        {
            if (opt == 2) { setPrintVerbose(s); }
            else if (opt == 1) { setPrintByteArray(s); }
            // this is the default print method
            else if (1 || opt == 0) { setPrintDefault(s); }
        }
    }
}

void setClear(set_t *s)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            setSetNumItems(s, 0);
            memset(setGetItems(s), 0, s->capacity);
        }
    }
}
