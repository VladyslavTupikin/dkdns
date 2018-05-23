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
#define CFG_PATH "/etc/dkdns/dkdns.cfg"
typedef struct dkdnscfg
{
    char* Local_IPv4;
    char* Main_IPv4;
    char* answer;
    
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
