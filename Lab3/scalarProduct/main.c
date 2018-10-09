#include <stdio.h>
#include <stdlib.h>

const int arrayA[] = {1,2,3,4,5};
const int arrayB[] = {5,4,3,2,1};

int scalarProduct(const int arrayA[], const int arrayB[], size_t count) {
	int result = 0;
	size_t i;
	for (i = 0; i < count; i++) {
		result += arrayA[i] * arrayB[i];
	}

	return result;
}

int main() {
	size_t size = sizeof(arrayA)/sizeof(int);
	int result = scalarProduct(arrayA, arrayB, size);
	printf("The result is %d\n", result);
	return 0;
}

