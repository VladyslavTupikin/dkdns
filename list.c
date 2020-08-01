#include "list.h"

List *init(char *data, size_t data_size)
{
    List *lst;
    lst = malloc(sizeof(List));

    if(!lst)
    {
        fprintf(stderr, "ERR: memory not allocated: lst_init\n");
        exit(FAILURE);
    }

    lst->dataPtr = malloc(data_size);

    if(!lst->dataPtr)
    {
        fprintf(stderr,"ERR: memory not allocated: lst_dataPtr_init\n");
        exit(FAILURE);
    }

    strcpy(lst->dataPtr,data);
    lst->nextPtr = NULL;
    return lst;
}

List *add(List *lst, char* data, size_t data_size)
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
    tmp->dataPtr = malloc(data_size);

    if(!tmp->dataPtr)
    {
        fprintf(stderr,"ERR: memory not allocated: lst->data_add\n");
        exit(FAILURE);
    }

    strcpy(tmp->dataPtr,data);
    tmp->nextPtr = saved;
    return tmp;
}





















