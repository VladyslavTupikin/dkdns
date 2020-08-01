/*
	Created by Vlad Tupikin
	vladtupikin7@gmail.com

*/
//----------------------------------------------------------
#ifndef DKDNS_H
#define DKDNS_H
//----------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
//----------------------------------------------------------
#include <arpa/inet.h>
#include <netinet/in.h>
//-----------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
//-----------------------------------------------------------
#include "cfgstruct.h"
//-----------------------------------------------------------
#define T_A 1
#define DGRAM_MX_SZE 65535
#define LOG_PATH "/var/log/dkdns.log"
#define PID_PATH "/var/run/dkdns.pid"
//-----------------------------------------------------------
int blck_lst(unsigned char* name,List* root);
int server(void);
void writelog(char* Msg,char* err);
void writelog_blst_name(char* Msg, char* err,char* addr);
void writelog_blst_ip(char* Msg,char* addr);
int blck_ip(unsigned char* ip,List* root);
//-----------------------------------------------------------
//DNS header structure
struct DNS_HEADER
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};
//---------------------------------------------------------------
#endif
