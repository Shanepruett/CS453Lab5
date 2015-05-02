
INSTALL=..
INCLUDE=$(INSTALL)/include -I.
LIBS=$(INSTALL)/
LFLAGS=-L$(LIBS) -lbuddy
CC=gcc
CFLAGS=-Wall -g  -I$(INCLUDE)

PROGS=buddy-simple-test malloc-test buddy-test buddy-modified-test buddy-unit-test

all: $(PROGS)

buddy-simple-test: buddy-simple-test.o
	$(CC) $(CFLAGS) -o $@ $^   $(LFLAGS)

buddy-unit-test: buddy-unit-test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

buddy-test: buddy-test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

buddy-modified-test: buddy-modified-test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

malloc-test: malloc-test.o
	$(CC) $(CFLAGS) -o $@ $^



clean:
	rm --force *.o $(PROGS)


