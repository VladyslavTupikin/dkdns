#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAILURE -1
typedef struct Lists
{
	struct Lists *nextPtr;
	char *dataPtr;
	
}List;

List *init(char *data);
List *add(List *prev,char* data);
#endif
