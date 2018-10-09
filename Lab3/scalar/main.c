#include <stdio.h>
#include <stdlib.h>

int scalarProduct(const int* arrayA, const int* arrayB, size_t count) {
	int result = 0;
	size_t i;
	for (i = 0; i < count; i++) {
		result += arrayA[i] * arrayB[i];
	}

	return result;
}

int main() {

	size_t size;
	printf("Enter array size\n");
	scanf("%d", &size);
	int* arrayA = (int*)malloc(size * sizeof(int));
	int* arrayB = (int*)malloc(size * sizeof(int));
	size_t i;
	printf("Enter the first array\n");
	for (i = 0; i < size; i++) {
		scanf("%d", &arrayA[i]);
	}
	printf("Enter the second array\n");
	for (i = 0; i < size; i++) {
		scanf("%d", &arrayB[i]);
	}
	int result = scalarProduct(arrayA, arrayB, size);
	printf("The result is %d\n", result);
	return 0;
}

