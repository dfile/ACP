// Implementation of set_t

#include <stdio.h>
#include <stdlib.h>
#include "set_t.h"

// Bare-bones initializer, just allocates memory and returns
set_t* setInit(void)
{
    return setInitWithLenAndZeroAndItems(0, 0, NULL);
}

// This is the initializer that should be used when making sets
set_t* setInitWithLenAndZero(unsigned int len, int zero)
{
    //byte *b = (byte *)calloc(len, sizeof(byte) * (len));
    // for some reason calloc can't allocate memory as large
    // as malloc can, so instead malloc the array and set
    // each index to be 0.
    byte *b = (byte *)malloc(sizeof(byte) * (len));
    int i;
    if (b == NULL) 
    {
        printf("Couldn't malloc in setInitWithLenAndZero()\n");
    }
    else
    {
        for (i = 0; i < len; i++)
        {
            b[i] = 0;
        }
    }
    set_t *s = setInitWithLenAndZeroAndItems(len, zero, b);
    return s;
}

// Use this initializer too
set_t* setInitWithRange(int low, int high)
{
    unsigned int len = high - low + 1;
    int zero = 0 - low;
    return setInitWithLenAndZero(len, zero);
}

// Designated initializer
set_t* setInitWithLenAndZeroAndItems(unsigned int len, int zero, byte *items)
{
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
    free(s);
}

//(byte *)malloc(sizeof(byte) * (len + zero))

byte* setGetItems(set_t *s)
{
    if (s != NULL) { return s->items; }
    else
    {
        printf("set_t NULL in setGetItems()\n");
        return NULL;
    }
}

void setSetItems(set_t *s, byte *b)
{
    if (s != NULL) { s->items = b; }
    else
    {
        printf("Trying to set items of NULL set in setSetItems()\n");
    }
}

unsigned int setGetNumItems(set_t *s)
{
    if (s != NULL) { return s->num_items; }
    else
    {
        printf("set_t NULL in setGetNumItems()");
        return 0;
    }
}

void setSetNumItems(set_t *s, unsigned int n)
{
    if (s != NULL) { s->num_items = n; }
    else
    {
        printf("Trying to set num_items of NULL set in setSetNumItems()\n");
    }
}

unsigned int setGetLen(set_t *s)
{
    if (s != NULL) { return s->len; }
    else
    {
        printf("set_t NULL in setGetLen()\n");
        return 0;
    }
}

void setSetLen(set_t *s, unsigned int len)
{
    if (s != NULL) { s->len = len; }
    else
    {
        printf("Trying to set len of NULL set in setSetLen()\n");
    }
}

int setGetZero(set_t *s)
{
    if (s != NULL) { return s->zero; }
    else
    {
        printf("set_t NULL in setGetZero()\n");
        return 0;
    }
}

void setSetZero(set_t *s, int zero)
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

int setGetLowRange(set_t *s)
{
    return (0 - setGetZero(s));
}

int setGetHighRange(set_t *s)
{
    return (setGetLen(s) - 1 - setGetZero(s));
}

char* setGetRange(set_t *s)
{
    char *r = (char *)malloc(sizeof(char) * 30);
    sprintf(r, "[%d to %d]", setGetLowRange(s), setGetHighRange(s));
    return r;
}

byte setExists(set_t *s, int e)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            int index = e + setGetZero(s);
            if (index >= 0 && index < setGetLen(s))
            {
                return (setGetItems(s)[index]);
            }
            else
            {
                char *range = setGetRange(s);
                printf("%d is outside of bounds %s of byte array of set in setExists()\n", e, range);
                free(range);
            }
        }
        else
        {
            printf("Can't check %d in NULL byte array of set in setExists()\n", e);
        }
    }
    else
    {
        printf("Can't check %d in NULL set in setExists()\n", e);
    }
    return (byte)(-1);

}

int setAdd(set_t *s, int a)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            int index = a + setGetZero(s);
            if (index >= 0 && index < setGetLen(s))
            {
                // increment num_items only if index doesn't exist (is 0)
                setSetNumItems(s, setExists(s, index - setGetZero(s)) ? setGetNumItems(s) : (setGetNumItems(s) + 1) );
                setGetItems(s)[index] = (byte)1;
                return 0;
            }
            else
            {
                char *range = setGetRange(s);
                printf("%d is outside of bounds %s of byte array of set in setAdd()\n", a, range);
                free(range);
            }
        }
        else
        {
            printf("Trying to add %d to NULL byte array of set in setAdd()\n", a);
        }
    }
    else
    {
        printf("Trying to add %d to NULL set in setAdd()\n", a);
    }
    return -1;
}

int setRemove(set_t *s, int r)
{
    if (s != NULL)
    {
        if (setGetItems(s) != NULL)
        {
            int index = r + setGetZero(s);
            if (index >= 0 && index < setGetLen(s))
            {
                // decrement num_items only if index exists (is 1)
                setSetNumItems(s, (setExists(s, index - setGetZero(s)) ? setGetNumItems(s) - 1 : setGetNumItems(s)) );
                setGetItems(s)[index] = (byte)0;
                return 0;
            }
            else
            {
                char *range = setGetRange(s);
                printf("%d is outside of bounds %s of byte array of set in setRemove()\n", r, setGetRange(s));
                free(range);
            }
        }
        else
        {
            printf("Can't remove %d from NULL byte array of set in setRemove()\n", r);
        }
    }
    else
    {
        printf("Can't remove %d from NULL set in setRemove()\n", r);
    }
    return -1;
}


void setPrintDefault(set_t *s)
{
    printf("Set items: [");
    int i = 0;
    for (i = 0; i < setGetLen(s); i++)
    {
        if (setExists(s, i - setGetZero(s)))
        {
            if (i + 1 == setGetLen(s))
            {
                printf("%d", i - setGetZero(s));
            }
            else
            {
                printf("%d,", i - setGetZero(s));
            }
        }
    }
    printf("]\n");
}

void setPrintByteArray(set_t *s)
{
    printf("Set items: [\n");
    int i = 0;
    for (i = 0; i < setGetLen(s); i++)
    {
        printf("%d: %d\n", i - setGetZero(s), (int)(setExists(s, i - setGetZero(s))));
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
        printf("Set num_items: %u\n", setGetNumItems(s));
        printf("Set zero: %d\n", setGetZero(s));
        printf("Set len: %u\n", setGetLen(s));
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
            int i = 0;
            for (i = 0; i < setGetLen(s); i++)
            {
                setGetItems(s)[i] = (byte)0;
            }
        }
    }
}
