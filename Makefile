CC=gcc
CFLAG=-I.
DEPS = dkdns.h list.h cfgstruct.h
OBJ=main.o server.o loadconf.o list.o 
AIM=dkdns
OPT= -O3
LIBS=-lconfig
%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

dknds: $(OBJ)
	$(CC) -g -o $(AIM) $(OBJ) $(CFLAG) $(LIBS) $(OPT)
clean:
	rm -f $(OBJ) $(AIM) 