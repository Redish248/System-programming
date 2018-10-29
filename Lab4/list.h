#pragma once
#include <stdlib.h>
#include <stdio.h>


typedef struct list {
	int value;
	struct list* next;
} list;

list* list_create(int); 
list* list_add_front(list** listL, int number); 
list* list_add_back(list* listL, int number); 
int list_get(list* link, int index); 
void list_free(list*); 
int list_length(list*);
list* list_node_at(list*, int); 
int list_sum(list*);
