#include <stdio.h>	// printf

void main(void)
{
	int i;
	int length = 5;

	for (i = 0; i < length; i++) {
		printf("%d", i);
		if (length == 5) {
			length = 10;
		}
	}
}
