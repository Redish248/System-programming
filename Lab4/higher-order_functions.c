#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "list.h"
#include "higher-order_functions.h"

//Accepts a pointer to the list start and a function(which returns void and accepts an int).It launches the function on each element of the list.
void foreach(list const* link, void(*f)(int)) {
	while (link != NULL) {
		f(link->value);
		link = link->next;
	}
}

//map accepts a function f and a list. It returns a new list containing the results of the f applied to all elements of the source list.
//The source list is not affected.
list* map(const list* link, int(*f)(int)) {
	list* linkedList = NULL;
	while (link->next != NULL) {
		linkedList = list_add_front(&linkedList, f(link->value));
		link = link->next;
	}
	linkedList = list_add_front(&linkedList, f(link->value));
	return linkedList;
}

//map_mut changes the source list.
void map_mut(list* link, int(*f)(int)) {
	while (link != NULL) {
		link->value = f(link->value);
		link = link->next;
	}
}

//
int foldl(int accumulator, int(*f)(int, int),list* link) {
	while (link != NULL) {
		accumulator = f(accumulator, link->value);
		link = link->next;
	}
	return accumulator;
}


list* iterate(int s, int length, int(*f)(int)) {
	list* linkedList = list_create(s);
	int i;
	for (i = 1; i < length; i++) {
		s = f(linkedList->value);
		linkedList = list_add_front(&linkedList, s);
	}
	return linkedList;
}

//write all elements of the list into a text file filename. It should return true in case the write is successful, false otherwise.
bool save(list* link, const char* filename) {
	FILE *myfile;
	myfile = fopen(filename, "w+");
	if (myfile == NULL) {
		return false;
	}
	while (link->next != NULL) {
		fprintf(myfile, "%d ", link->value);
		link = link->next;
	}
	fprintf(myfile, "%d ", link->value);
	fclose(myfile);
	return true;
}

//read all integers from a text file filename and write the saved list into *lst.It should return true in case the write is successful, false otherwise.
bool load(list** link, const char* filename) {
	FILE *myfile;
	myfile = fopen(filename, "r");
	if (myfile == NULL) {
		return false;
	}
	int value;
	while (fscanf(myfile, "%d", &value)!= EOF) {
		*link = list_add_front(link, value);
	}
	fclose(myfile);
	return true;
}

//which will read all integers from a binary file filename and write
//the saved list into *lst.It should return true in case the write is successful, false otherwise.
bool deserialize(list** link, const char* filename) {
	FILE *myfile = fopen(filename, "r");

	fseek(myfile, 0, SEEK_END);
	long sz = ftell(myfile), i;
	rewind(myfile);

	int* buf = (int*)malloc(sz * sizeof(int));
	fread(buf, sz / sizeof(int), sizeof(int), myfile);

	for (i = 0; i < sz / sizeof(int); ++i) {
		*link = list_add_front(link, *(buf + i));
	}

	fclose(myfile);
	return true;
}

//write all elements of the list into a binary file filename. It should return true in case the write is successful, false otherwise.
bool serialize(list** link, const char*filename) {
	FILE *myfile = fopen(filename, "w+");
	int number;

	if (myfile == NULL) {
		return false;
	}

	size_t sz = list_length(*link), i;
	int* buf = (int*)malloc(sz * sizeof(int));
	for (i = 0; i < sz; ++i) {
		*(buf + i) = (*link)->value;
		*link = (*link)->next;
	}
	fwrite(buf, sz, sizeof(int), myfile);

	fclose(myfile);
	return true;
}