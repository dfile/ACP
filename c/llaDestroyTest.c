#include <stdio.h>      // printf
#include <stdlib.h>     // malloc
#include <string.h>     // memcpy
#include "LinkedListArray.h"
#include "typedefs.h"

void main(void) {
  number arr[4] = {1, 2, 3, 4};
  lla* list = llaInit();
  llaAppend(list, nodeArrayInitWithArray(arr));
  llaDestroy(list);
}
