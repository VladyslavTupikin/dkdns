#include "list.h"

List *init(char *data)
{
    List *lst;
    lst = malloc(sizeof(List));

    if(!lst)
    {
        fprintf(stderr, "ERR: memory not allocated: lst_init\n");
        exit(FAILURE);
    }

    strcpy(lst->dataPtr,data);
    lst->nextPtr = NULL;
    return lst;
}

List *add(List *lst, char* data)
{
    List *tmp, *saved;
    tmp = malloc(sizeof(List));
    if(!tmp)
    {
        fprintf(stderr,"ERR: memory not allocated: tmp_add\n");
        exit(FAILURE);
    }
    saved =(List*)lst->nextPtr;
    lst->nextPtr = tmp;

    strcpy(tmp->dataPtr,data);
    tmp->nextPtr = saved;
    return tmp;
}





















