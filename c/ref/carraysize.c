#include <stdio.h>

void main(void) {

	int array[5][10] = {0};

	int rows = sizeof(array) / sizeof(array[0]);
	int cols = sizeof(array[0]) / sizeof(array[0][0]);

	printf("array has %d rows \n", rows);
	printf("array has %d cols \n", cols);
}
