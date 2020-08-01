CC=gcc
CFLAGS=-std=c11 -D_DEFAULT_SOURCE
DEPS=dkdns.h list.h cfgstruct.h
OBJ=main.o server.o loadconf.o list.o
AIM=dkdns
OPT=-O3
LIBS=-lconfig

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

dknds: $(OBJ)
	$(CC) -g -o $(AIM) $(OBJ) $(CFLAGS) $(LIBS) $(OPT)
clean:
	rm -f $(OBJ)
clean_all:
	rm -r $(OBJ) $(AIM)
