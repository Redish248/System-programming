#include "list.h"

//accepts a number, returns a pointer to the new linked list node.
list* list_create(int number) {
	list* linkedList = (list*)malloc(sizeof(list));
	linkedList->value = number;
	linkedList->next = NULL;
	return linkedList;
}

// accepts a number and a pointer to a pointer to the linked list. Prepends the new node with a number to the list.
list* list_add_front(list** listL, int number) {
	if (listL == NULL) {
		return list_create(number);
	}
	list* linkedList = (list*)malloc(sizeof(list));
	linkedList->value = number;
	linkedList->next = *listL;
	return linkedList;
}


//adds an element to the end of the list.The signature is the same as list_add_front.
list* list_add_back(list* listL, int number) {
	if (listL == NULL) {
		return list_create(number);
	}
	while (listL->next != NULL) {
		listL = listL->next;
	}
	list* linkedList = (list*)malloc(sizeof(list));
	linkedList->value = number;
	linkedList->next = NULL;
	listL->next = linkedList;
	return listL;
}

// gets an element by index, or returns 0 if the index is outside the list bounds.
int list_get(const list* link, int index) {
	list* l = list_node_at(link, index);
	if (l == NULL) {
		return 0;
	} else {
		return l->value;
	}
}

// frees the memory allocated to all elements of list.
void list_free(list* link) {
	while (link != NULL) {
		list* fr = link->next;
		free(link);
		link = fr;
	}
}

// accepts a list and computes its length.
int list_length(list* link) {
	int length = 0;
	while (link->next != NULL) {
		link = link->next;
		length++;
	}
	length++;
	return length;
}

// accepts a list and an index, returns a pointer to struct list, corresponding to the node at this index.If the index is too big, returns NULL.
list* list_node_at(const list* link, int index) {
	int n = 0;
	list* linkedList = link;
	while ((linkedList->next != NULL) && (n < index)){
		linkedList = linkedList->next;
		n++;
	}
	if (n != index) {
		printf("out of bounds!");
		return NULL;
	}
	else {
		return linkedList;
	}
}

//a function to compute the sum of elements in a linked list.
int list_sum(list const* listL) {
	if (listL == NULL) {
		return 0;
	}
	int sum = 0;
	list* linkedList = listL;
	while (linkedList->next != NULL) {
		sum += linkedList->value;
		linkedList = linkedList->next;
	}
	sum += linkedList->value;
	return sum;
}