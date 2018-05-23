/*
	Created by Vlad Tupikin
	vladtupikin7@gmail.com

*/

#include "dkdns.h"
#define START "/dkdns"

int main(int argc, char** argv)
{

	int pid,status;

	if(argc >= 2)
	{
		if(strcmp(argv[1],"start")==0)
		{
			pid = fork();
			if (pid == -1) 
    		{
				printf("[DKDNS]: Start Daemon failed (%s)", strerror(errno));
				return -1;
			}
			else if (!pid)
			{
				FILE* fd;
				FILE* fd2;
				fd2 = fopen(PID_PATH,"r");
				if(fd2)
				{
					printf("[DKDNS]: Server works.Abort.\n");
					writelog("[DKDNS]:","Server works.Abort.");
					exit(0);
				}
				
				errno = 0;
				

				writelog("[DKDNS]: Started: ", strerror(errno));
				fd = fopen(PID_PATH, "w+");
				if(!fd)
				{
					writelog("[DKDNS]: PID: ", strerror(errno));
				}
				fprintf(fd,"%u\n",getpid());
				fclose(fd);
				umask(0);
				setsid();
				chdir("/");
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				close(STDERR_FILENO);				
				status = server();
			}						
		}
		else if(strcmp(argv[1],"stop")==0)
		{
			FILE* fd = fopen(PID_PATH, "r");
			if(!fd)
			{
				writelog("[DKDNS]: fopen pid: ", strerror(errno));
				return -1;
			}
			fscanf(fd,"%d", &pid);
			fclose(fd);

			if(kill(pid,SIGKILL) < 0)
			{
				writelog("[DKDNS]: kill: ", strerror(errno));
				
				if(unlink(PID_PATH) < 0)
				{	
					writelog("[DKDNS]: unlink: ", strerror(errno));
				}

				return -1;	
			}
			if(unlink(PID_PATH) < 0)
			{	
				writelog("[DKDNS]: unlink: ", strerror(errno));
			}
			writelog("[DKDNS]: Stopped: ", strerror(errno));
		}
		else if(strcmp(argv[1],"restart")==0)
		{
			char buf[128];
			FILE* fd = fopen(PID_PATH, "r");
			if(fd == NULL)
			{
				writelog("[DKDNS]: fopen pid: ", strerror(errno));
				return -1;
			}

			fscanf(fd,"%d", &pid);
			fclose(fd);	

			if(kill(pid,SIGKILL) < 0)
			{
				writelog("[DKDNS]: kill: ", strerror(errno));
								
				if(unlink(PID_PATH) < 0)
				{	
					writelog("[DKDNS]: unlink: ", strerror(errno));
				}

				return -1;		
			}
			if(unlink(PID_PATH) < 0)
			{	
				writelog("[DKDNS]: unlink: ", strerror(errno));
			}
			char* path = getcwd(buf,sizeof(buf));
			if(!path)
			{
				writelog("[DKDNS]: getcwd: ", strerror(errno));
			}
			writelog("[DKDNS]: Restart... ","");
			int ret = execl(strcat(path,START),"dkdns","start",(char*)NULL);
			if(ret < 0)
			{
				writelog("[DKDNS]: execl: ", strerror(errno));
				return -1;
			}
		}
		else
		{
			printf("Usage: dkdns command(commands: start,stop,restart)\n");
			return -1;
		}				
	}




	return 0;
}
/*
	Created by Vlad Tupikin
	vladtupikin7@gmail.com
	
*/
