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
    //byte *b = (byte *)calloc(len, sizeof(byte) * (len));
    // for some reason calloc can't allocate memory as large
    // as malloc can, so instead malloc the array and set
    // each index to be 0.
    //printf("malloc set items: %lld bytes\n", sizeof(byte) * len);
    byte *b = (byte *)malloc(sizeof(byte) * (len));
    //number i;
    if (b == NULL) 
    {
        printf("ERR: Couldn't malloc in setInitWithLenAndZero()\n");
    }
    else
    {
        memset(b, 0, len);
        //for (i = 0; i < len; i++)
        //{
        //    b[i] = 0;
        //}
    }
    set_t *s = setInitWithLenAndZeroAndItems(len, zero, b);
    return s;
}

// Use this initializer too
set_t* setInitWithRange(number low, number high)
{
    unumber len = high - low + 1;
    number zero = 0 - low;      // TODO: make this work for positive lows
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

//(byte *)malloc(sizeof(byte) * (len + zero)) 
byte* setGetItems(set_t *s)
{
    if (s != NULL) { return s->items; }
    else
    {
        printf("ERR: set_t NULL in setGetItems()\n");
        return NULL;
    }
}

void setSetItems(set_t *s, byte *b)
{
    if (s != NULL) { 
        if (b == NULL) { printf("ERR: Trying to set NULL items to set in setSetItems()\n"); }
        s->items = b;
    }
    else
    {
        printf("ERR: Trying to set items of NULL set in setSetItems()\n");
    }
}

unumber setGetNumItems(set_t *s)
{
    if (s != NULL) { return s->num_items; }
    else
    {
        printf("set_t NULL in setGetNumItems()");
        return 0;
    }
}

void setSetNumItems(set_t *s, unumber n)
{
    if (s != NULL) { s->num_items = n; }
    else
    {
        printf("Trying to set num_items of NULL set in setSetNumItems()\n");
    }
}

unumber setGetLen(set_t *s)
{
    if (s != NULL) { return s->len; }
    else
    {
        printf("set_t NULL in setGetLen()\n");
        return 0;
    }
}

void setSetLen(set_t *s, unumber len)
{
    if (s != NULL) { s->len = len; }
    else
    {
        printf("Trying to set len of NULL set in setSetLen()\n");
    }
}

number setGetZero(set_t *s)
{
    if (s != NULL) { return s->zero; }
    else
    {
        printf("set_t NULL in setGetZero()\n");
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
        printf("Trying to set zero of NULL set in setSetZero()\n");
        return;
    }
}

number setGetLowRange(set_t *s)
{
    return (0 - setGetZero(s));
}

number setGetHighRange(set_t *s)
{
    return (setGetLen(s) - 1 - setGetZero(s));
}

// XXX: pass in a buffer that it fills in rather than malloc
char* setGetRange(set_t *s)
{
    char *r = (char *)malloc(sizeof(char) * 30);
    sprintf(r, "[%lld to %lld]", setGetLowRange(s), setGetHighRange(s));
    return r;
}

byte setExists(set_t *s, number e)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            number index = e + setGetZero(s);
            if (index >= 0 && index < setGetLen(s))
            {
                return (setGetItems(s)[index]);
            }
            else
            {
                char *range = setGetRange(s);
                printf("ERR: %lld is outside of bounds %s of byte array of set in setExists()\n", e, range);
                free(range);
            }
        }
        else
        {
            printf("ERR: Can't check %lld in NULL byte array of set in setExists()\n", e);
        }
    }
    else
    {
        printf("ERR: Can't check %lld in NULL set in setExists()\n", e);
    }
    return (byte)(-1);

}

number setAdd(set_t *s, number a)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            number index = a + setGetZero(s);
            if (index >= 0 && index < setGetLen(s))
            {
                // increment num_items only if index doesn't exist (is 0)
                //printf("Adding %lld at %lld to set\n", a, index);
                setSetNumItems(s, setExists(s, index - setGetZero(s)) ? setGetNumItems(s) : (setGetNumItems(s) + 1) );
                setGetItems(s)[index] = (byte)1;
                return 0;
            }
            else
            {
                char *range = setGetRange(s);
                printf("ERR: %lld is outside of bounds %s of byte array of set in setAdd()\n", a, range);
                free(range);
            }
        }
        else
        {
            printf("ERR: Trying to add %lld to NULL byte array of set in setAdd()\n", a);
        }
    }
    else
    {
        printf("ERR: Trying to add %lld to NULL set in setAdd()\n", a);
    }
    return -1;
}

number setRemove(set_t *s, number r)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            number index = r + setGetZero(s);
            if (index >= 0 && index < setGetLen(s))
            {
                // decrement num_items only if index exists (is 1)
                printf("Removing %lld at %lld from set\n", r, index);
                setSetNumItems(s, (setExists(s, index - setGetZero(s)) ? setGetNumItems(s) - 1 : setGetNumItems(s)) );
                setGetItems(s)[index] = (byte)0;
                return 0;
            }
            else
            {
                char *range = setGetRange(s);
                printf("ERR: %lld is outside of bounds %s of byte array of set in setRemove()\n", r, setGetRange(s));
                free(range);
            }
        }
        else
        {
            printf("ERR: Can't remove %lld from NULL byte array of set in setRemove()\n", r);
        }
    }
    else
    {
        printf("ERR: Can't remove %lld from NULL set in setRemove()\n", r);
    }
    return -1;
}


void setPrintDefault(set_t *s)
{
    printf("Set items: [");
    number i = 0;
    for (i = 0; i < setGetLen(s); i++)
    {
        if (setExists(s, i - setGetZero(s)))
        {
            if (i + 1 == setGetLen(s))
            {
                printf("%lld", i - setGetZero(s));
            }
            else
            {
                printf("%lld,", i - setGetZero(s));
            }
        }
    }
    printf("]\n");
}

void setPrintByteArray(set_t *s)
{
    printf("Set items: [\n");
    number i = 0;
    for (i = 0; i < setGetLen(s); i++)
    {
        printf("%lld: %lld\n", i - setGetZero(s), (number)(setExists(s, i - setGetZero(s))));
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
        printf("Set num_items: %llu\n", setGetNumItems(s));
        printf("Set zero: %lld\n", setGetZero(s));
        printf("Set len: %llu\n", setGetLen(s));
        char *range = setGetRange(s);
        printf("Set range: %s\n", range);
        free(range);

        if (setGetItems(s) == NULL) { printf("Set items: NULL\n"); }
        else
        {
            if (opt == 2) { setPrintVerbose(s); }
            else if (opt == 1) { setPrintByteArray(s); }
            // this is the default print method
            else if (opt == 0 || 1) { setPrintDefault(s); }
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
            //number i = 0;
            memset(setGetItems(s), 0, s->len);
            //for (i = 0; i < setGetLen(s); i++)
            //{
            //    setGetItems(s)[i] = (byte)0;
            //}
        }
    }
}
