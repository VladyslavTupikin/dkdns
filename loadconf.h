//-----------------------
#ifndef CFGSTRUCT_H
#define CFGSTRUCT_H
//-----------------------
#include <libconfig.h>
#include <string.h>
#include <unistd.h>
//-----------------------
#include "list.h"
//-----------------------
#define SIZE_IPv4 4
#define SIZE_ANSWER 20
//-----------------------

#ifndef CFG_PATH
    #define CFG_PATH "/etc/dkdns/dkdns.cfg"
#endif

typedef struct dkdnscfg
{
    char Local_IPv4[SIZE_IPv4];
    char Main_IPv4[SIZE_IPv4];

    int Local_Port;
    int Main_Port;
    int msg;
    int wtime;

    List* blck_name_lst;
    List* blck_ip_lst;
}DKconf;
//-----------------------
DKconf* loadcfg(void);
//-----------------------
#endif
//-----------------------
