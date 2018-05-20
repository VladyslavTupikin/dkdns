	ATTANTION: PROJECT NOT COMPLETED!
	DKDNS - special dns proxy server with blacklist.

	To compile dkdns, you need: gcc, libconfig (http://hyperrealm.com/oss_libconfig.shtml).
	After just use in command line make. DKDNS will be compiled.

	Then, change configuration file (dkdns.cfg) and put it in 
	/etc/dkdns/ folder (if you want to change path to dkdns.cfg,
	open cfgstruct.h,find CFG_PATH and change "/etc/dkdns/dkdns.cfg"
	to your path, again use make to recompile dkdns).

	Log files write in /var/log/dkdns.log (if you need to change path
	open dkdns.h,find LOG_PATH and change "/var/log/dkdns.log" to your
	path, then use make to recompile dkdns).

	For start DKDNS, use command: sudo ./dkdns start.
	For stop DKDNS, use command: sudo ./dkdns stop.
	For restart DKDNS, use command: sudo ./dkdns stop.

	If you changed some params in dkdns.cfg, just restart dkdns.

	DKDNS passed tests on Linux Mint 18.2.
	uname -a: Linux dk-pc 4.4.0-53-generic #74-Ubuntu SMP Fri Dec 2 15:59:10 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux

	
