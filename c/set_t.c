// Implementation of set_t

set_t* setInit(void)
{
    return setInitWithLenAndZeroAndItems(NULL, 0, 0);
}

set_t* setInitWithLenAndZero(unsigned int len, unsigned int zero)
{
    return setInitWithLenAndZeroAndItems((byte *)malloc(sizeof(byte) * (len + zero)), len, zero);
}

set_t* setInitWithLenAndZeroAndItems(byte *items, unsigned int len, unsigned int zero)
{
    set_t *set = malloc(sizeof(struct set_s));
    setSetItems(set, items);
    setSetNumItems(set, 0);
    setSetZero(set, zero);
    setSetLen(set, len);
    return set;
}

//(byte *)malloc(sizeof(byte) * (len + zero))
