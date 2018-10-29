#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "list.h"
#include "higher-order_functions.h"

void printWithSpace(int val) {
	printf("%d ", val);
}

void printNextLine(int val) {
	printf("%d\n", val);
}

int sqr(int x) {
	return x * x;
}

int cube(int x) {
	return x * x * x;
}

int module(int x) {
	return abs(x);
}

int count_2(int n) {
	return n * 2;
}

int sumFoldl(int sum, int x) {
	return sum + x;
}

int maxF(int max, int x) {
	if (x > max) {
		return x;
	}
	else {
		return max;
	}
}

int minF(int min, int x) {
	if (x < min) {
		return x;
	}
	else {
		return min;
	}
}


int main() {
	list* linkedList =  NULL;
	
	int m = 0;
	scanf("%d", &m);
	printf("enter list:\n");
	int n;
	int i = 0;
	while (i < m) {
		scanf("%d", &n);
		linkedList = list_add_front(&linkedList, n);
		i++;
	}
	//============================================first part
	printf("Sum:\n");
	int s = list_sum(linkedList);
	printf("%d\n", s);

	printf("Length:\n");
	int length = list_length(linkedList);
	printf("%d\n", length);

	int index;
	printf("Enter index!\n");
	scanf("%d", &index);
	printf("Value:\n");
	int value = list_get(linkedList, index);
	printf("%d\n", value);
	//============================================first part

	printf("List with spaces:\n");
	foreach(linkedList,&printWithSpace);

	printf("\nList in lines:\n");
	foreach(linkedList, &printNextLine);

	printf("SQR:\n");
	list* newList = map(linkedList, &sqr);
	foreach(newList, &printWithSpace);

	printf("\nCubes:\n");
	newList = map(linkedList, &cube);
	foreach(newList, &printWithSpace);

	printf("\nSum2:\n");
	int sum2 = foldl(0, &sumFoldl, linkedList);
	printf("%d\n", sum2);

	printf("\nMax:\n");
	int max2 = foldl(INT_MIN, &maxF, linkedList);
	printf("%d\n", max2);

	printf("\nMin:\n");
	int min2 = foldl(INT_MAX, &minF, linkedList);
	printf("%d\n", min2);

	printf("\nModules:\n");
	map_mut(linkedList, &module);
	foreach(linkedList, &printWithSpace);

	printf("\n2^n:\n");
	list* list2 = iterate(2, 10, &count_2);
	foreach(list2, &printWithSpace);

	printf("\nYou can see all linked list in out.txt\n");
	save(linkedList, "out.txt");

	printf("\nLoad list from in.txt\n");
	list* listLoad = NULL;
	load(&listLoad, "in.txt");
	foreach(listLoad, &printWithSpace);

	printf("\nSerialize&Deserialize\n");
	list* ser= NULL;
	serialize(&linkedList,"output.bin");
	deserialize(&ser, "output.bin");
	foreach(ser, &printWithSpace);

	list_free(listLoad);
	list_free(newList);
	list_free(linkedList);
	return 0;
}