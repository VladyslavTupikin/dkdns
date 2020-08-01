/*
    Created by Vlad Tupikin
    vladtupikin7@gmail.com

*/

#include "dkdns.h"

int server(void)
{
/*
        buf - main buffer with query
        buf_sv - saving main buffer for blacklist checking
        *qname - to get DNS header from UDP package
        master_sock - for binding local server ip,
                      and exchanging queries and answers
                      with client
        slave_sock - for exchanging queries and answers
                     with main server.
        bytes - saving query bytes from client
                saving answer bytes from main server

*/

    unsigned char buf[DGRAM_MX_SZE],*qname, buf_sv[sizeof(buf)];
    int master_sock,slave_sock;
    int bytes;
    struct sockaddr_in todns;
    struct sockaddr_in local;
    struct sockaddr_in src;
    struct sockaddr_in save;
    int src_len;
    struct timeval tv;
    DKconf* tmp;
    DKconf* confPtr;
 /*
        Load configurations from
        dkdns.cfg
 */
    confPtr = loadcfg();
    if(!confPtr)
    {
        writelog("[SRV]:   loadcfg: ", strerror(errno));
        return -1;
    }
    tmp = confPtr;

/*
        Clean buffers;
        Create master socket,bind it to local ip.
        Create slave socket for main server ip.
        Set options for slave socket: waiting
        answer time 1 second.
*/
    memset(buf,0,sizeof(buf));
    memset(buf_sv,0,sizeof(buf_sv));

    master_sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);

     if(master_sock < 0)
    {
        writelog("[SRV]:   master_sock: ", strerror(errno));
        return -1;
    }

    local.sin_family = AF_INET;
    local.sin_port = htons(tmp->Local_Port);
    inet_aton(tmp->Local_IPv4,&local.sin_addr);

    if(bind(master_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        writelog("[SRV]:   bind: ", strerror(errno));
        return -1;
    }

    todns.sin_family = AF_INET;
    todns.sin_port = htons(tmp->Main_Port);
    inet_aton(tmp->Main_IPv4,&todns.sin_addr);

    slave_sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);

    if(slave_sock < 0)
    {
        writelog("[SRV]:   slave_sock: ", strerror(errno));
        return -1;
    }

    tv.tv_sec = tmp->wtime;
    tv.tv_usec = 0;

    if (setsockopt(slave_sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
    {
        writelog("[SRV]:   setsockopt: ", strerror(errno));
        return -1;
    }


/*
        Get query from client.
        Check domain name in blacklist.
        If blk_lst ret 1, domain name in blacklist,
        server will wait for new query from client.
        Else if blk_lst ret value 0, then server
        send query to main server, get answer and
        send it to client
*/

    writelog("[SRV]:   Started: ", strerror(errno));
        static short ipcount = 0;
    while(1)
    {
        int time_s = time(NULL);
        bytes = 0;
        memset(buf,0,sizeof(buf));

        bytes = recvfrom(master_sock, (unsigned char*)buf , DGRAM_MX_SZE,
                     0, (struct sockaddr*)&src, &src_len);

        if(bytes < 0)
        {
            writelog("[SRV]: master_sock: recvfr:", strerror(errno));
            continue;
        }
        memcpy(buf_sv,buf,sizeof(buf));
        src_len = sizeof(src);
        qname = (unsigned char*)&buf_sv[sizeof(struct DNS_HEADER)];

        if(blck_lst(qname,tmp->blck_name_lst))
        {
            memset(buf,0,sizeof(buf));
            int time_ip = time(NULL);
            if((time_ip-time_s) > 1)
                 writelog_blst_name("[SRV]: name from blacklist:", qname,inet_ntoa(src.sin_addr));

            if(tmp->msg == 1)
                if(sendto(master_sock,(char*)buf,bytes,0,(struct sockaddr*)&src, src_len)<0)
                    writelog("[SRV]: blacklist: sendto:", strerror(errno));
            continue;
        }

        if(blck_ip(inet_ntoa(src.sin_addr),tmp->blck_ip_lst))
        {
            memset(buf,0,sizeof(buf));
            int time_ip = time(NULL);

            if((time_ip-time_s) < 2)
                continue;

            writelog_blst_ip("[SRV]: ip from blacklist:", inet_ntoa(src.sin_addr));

            ipcount++;
            if(tmp->msg == 1)
                if(sendto(master_sock,(char*)buf,bytes,0,(struct sockaddr*)&src, src_len)<0)
                    writelog("[SRV]: blacklist: sendto:", strerror(errno));
            continue;
        }
/*
        if(strcmp(inet_ntoa(src.sin_addr),inet_ntoa(save.sin_addr))==0)
        {
            int time_c = time(NULL);
            if((time_s-time_c) <=2)
                ipcount++;
            if(ipcount >= 1)
            {
                List* temp = tmp->blck_ip_lst;
                while(temp->nextPtr!=NULL)
                {
                    temp = temp->nextPtr;
                }
                if(strcmp(inet_ntoa(src.sin_addr),tmp->Local_IPv4)!=0)
                {
                    add(temp,inet_ntoa(src.sin_addr));
                    writelog_blst_ip("[SRV]: ip added to blacklist:", inet_ntoa(src.sin_addr));
                    ipcount = 0;
                    continue;
                }

            }
        }
        ipcount -= 1;

        save.sin_addr = src.sin_addr;
*/
        if(sendto(slave_sock,(char*)buf,bytes,0,(struct sockaddr*)&todns, sizeof(todns))<0)
        {
            writelog("[SRV]: slave_sock: sendto:", strerror(errno));
            continue;
        }

        bytes = recvfrom(slave_sock, (char*)buf , DGRAM_MX_SZE,
                        0,(struct sockaddr*)&todns, (socklen_t*)&src_len);
        if(bytes < 0)
        {
            writelog("[SRV]: slave_sock: recvfr:", strerror(errno));
            continue;
        }

        if(sendto(master_sock,(char*)buf,bytes,0,(struct sockaddr*)&src, src_len)<0)
        {
            writelog("[SRV]: master_sock: sendto:", strerror(errno));
            continue;
        }
    }
}
/*
    Write logs to LOG_PATH
*/
void writelog(char* Msg, char* err)
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    FILE *fd = NULL;
    fd = fopen(LOG_PATH,"a");
    if(fd == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fd,"%s %s %s",Msg,err,asctime(timeinfo));
    fclose(fd);
}

void writelog_blst_name(char* Msg, char* err,char* addr)
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    FILE *fd = NULL;
    fd = fopen(LOG_PATH,"a");
    if(fd == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fd,"%s %s from ip: %s\n[***]: %s",Msg,err,addr,asctime(timeinfo));
    fclose(fd);
}

void writelog_blst_ip(char* Msg,char* addr)
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    FILE *fd = NULL;
    fd = fopen(LOG_PATH,"a");
    if(fd == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fd,"%s %s\n[***]: %s",Msg,addr,asctime(timeinfo));
    fclose(fd);
}
/*
        Convert name from DNS form (2vk3com) to normal (vk.com)
        and compare with blacklist
*/

int blck_lst(unsigned char* name,List* root)
{
    unsigned char* qname = name;
    unsigned int p = 0;int i = 0,j=0;

    for(i=0;i<(int)strlen((const char*)qname);i++)
    {
        p=qname[i];
        for(j=0;j<(int)p;j++)
        {
           qname[i]=qname[i+1];
            i=i+1;
        }
        qname[i]='.';
    }
    qname[i-1]='\0';
    List* tmp = root;
    while(tmp != NULL)
    {
        if(strcmp(qname,tmp->dataPtr) == 0)
            return 1;
        tmp=tmp->nextPtr;
    }
    return 0;
}

int blck_ip(unsigned char* ip,List* root)
{
    unsigned char* ipname = ip;

    List* tmp = root;
    while(tmp != NULL)
    {
        if(strcmp(ipname,tmp->dataPtr) == 0)
            return 1;
        tmp=tmp->nextPtr;
    }
    return 0;
}
/*
    Created by Vlad Tupikin
    vladtupikin7@gmail.com

*/
