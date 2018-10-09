#include <stdio.h>
#include <stdlib.h>

int isPrime(unsigned long number) {
	size_t i = 2;
	while (i*i <= number) {
		if ((number % i) == 0) {
			return 0;
		}
		i++;
	}
		return 1;
}

int main() {
	unsigned long number;
	printf("Enter the number\n");
	scanf("%lu", &number);
	int result = isPrime(number);
	if (result == 1) {
		printf("This is a prime number\n");
	} else {
		printf("This is a composite number\n");
	}
	return 0;
}