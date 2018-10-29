#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


void foreach(list const* link, void(*f)(int));
list* map(const list*, int(*f)(int));
void map_mut(list*, int(*f)(int));
int foldl(int accumulator, int(*f)(int, int), list* link);
list* iterate(int s, int length, int(*f)(int));
bool save(list* link, const char* filename); 
bool load(list** link, const char* filename); 
bool serialize(list** link, const char* filename); 
bool deserialize(list** link, const char*filename); 

