//-----------------------
#include "cfgstruct.h"
#include "list.h"
//-----------------------
#define SIZE_IPv4 4
#define SIZE_ANSWER 20
//-----------------------

/*
    Configuration file handler
    set configurations in /etc/dkdnf.cfg
*/
DKconf* loadcfg(void)
{
    config_t cfg;
    config_setting_t *setting;
    const char *str;
    const char* tmpstr;
    DKconf *confdk;
    List* tmp;

    confdk = malloc(sizeof(DKconf));
    if(!confdk)
    {
        perror("malloc: ");
        return NULL;
    }
    config_init(&cfg);

    if(!config_read_file(&cfg, CFG_PATH))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                    config_error_line(&cfg), config_error_text(&cfg));
       return NULL;
    }

    if(!config_lookup_string(&cfg, "Local_IPv4", &str))
    {
        fprintf(stderr, "No 'Local_IPv4' setting in configuration file.");
        return NULL;
    }
    confdk->Local_IPv4 = malloc(SIZE_IPv4);
    strcpy(confdk->Local_IPv4,str);

    if(!config_lookup_int(&cfg, "Local_Port", &confdk->Local_Port))
    {
        fprintf(stderr, "No 'Local_Port' setting in configuration file.");
        return NULL;
    }

    if(!config_lookup_string(&cfg, "Main_IPv4", &str))
    {
        fprintf(stderr, "No 'Main_IPv4' setting in configuration file.");
        return NULL;
    }
    confdk->Main_IPv4 = malloc(SIZE_IPv4);
    strcpy(confdk->Main_IPv4,str);

    if(!config_lookup_int(&cfg, "Main_Port", &confdk->Main_Port))
    {
        fprintf(stderr, "No 'Main_Port' setting in configuration file.");
        return NULL;
    }

    if(!config_lookup_int(&cfg, "msg", &(confdk->msg)))
    {
        fprintf(stderr, "No 'msg' setting in configuration file.");
        return NULL;
    }

    if(!config_lookup_int(&cfg, "wtime", &(confdk->wtime)))
    {
        fprintf(stderr, "No 'wtime' setting in configuration file.");
        return NULL;
    }

    if(!config_lookup_string(&cfg, "answer", &str))
    {
        fprintf(stderr, "No 'answer' setting in configuration file.");
        return NULL;
    }
    confdk->answer = malloc(SIZE_ANSWER);
    strcpy(confdk->answer,str);

    setting = config_lookup(&cfg,"blacklist_domain");

    if(setting)
    {
        unsigned int size = config_setting_length(setting);

        for(int i = 0;i < size;i++)
        {
            if(i==0)
            {
                tmpstr = config_setting_get_string_elem(setting,i);
                confdk->blck_name_lst = init((char*)tmpstr,strlen(tmpstr) + 1);
                tmp = confdk->blck_name_lst;
            }
            else
            {
                tmpstr = config_setting_get_string_elem(setting,i);
                tmp = add(tmp,(char*)tmpstr, strlen(tmpstr) + 1);
            }
        }
    }

    setting = config_lookup(&cfg, "blacklist_ip");

    if(setting)
    {
        unsigned int size = config_setting_length(setting);

        for(int i = 0;i < size;i++)
        {
            if(i==0)
            {
                tmpstr = config_setting_get_string_elem(setting, i);
                confdk->blck_ip_lst = init((char*)tmpstr, strlen(tmpstr) + 1);
                tmp = confdk->blck_ip_lst;
            }
            else
            {
                tmpstr = config_setting_get_string_elem(setting, i);
                tmp = add(tmp, (char*)tmpstr, strlen(tmpstr) + 1);
            }
        }
    }

    config_destroy(&cfg);
    return confdk;
}
