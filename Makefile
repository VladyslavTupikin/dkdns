CC=gcc
CFLAGS=-std=c99 -D _POSIX_C_SOURCE -D__BSD_SOURCE -D_SVID_SOURCE
DEPS=dkdns.h list.h cfgstruct.h
OBJ=main.o server.o loadconf.o list.o 
AIM=dkdns
OPT=-O3
LIBS=-lconfig

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

dknds: $(OBJ)
	$(CC) -g -o $(AIM) $(OBJ) $(CFLAGS) $(LIBS) $(OPT)
	make clean
clean:
	rm -f $(OBJ)
clean_all:
	rm -r $(OBJ) $(AIM)	
