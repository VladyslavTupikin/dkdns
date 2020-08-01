#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAILURE -1
#define LIST_DATA_SIZE 2048

typedef struct Lists
{
    struct Lists *nextPtr;
    char dataPtr[LIST_DATA_SIZE];

}List;

List *init(char *data);
List *add(List *prev,char* data);
int erase(List** head);

#endif
