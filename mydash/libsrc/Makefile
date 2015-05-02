INSTALL=..
LIB=$(INSTALL)
INCLUDE=$(INSTALL)/include
CC=gcc
CFLAGS=-Wall -g -fPIC -shared -I$(INCLUDE)/
LFLAGS=-L$(LIB) -lmylib -lbuddy


LIBOBJS=List.o Node.o
BUDDYOBJS=buddy.o

all: libs 


libs: libmylib.so  libmylib.a libbuddy.so libbuddy.a

libmylib.so: $(LIBOBJS)
	$(LD) -shared -fPIC -o $@  $(LIBOBJS)

libmylib.a: $(LIBOBJS)
	$(AR)  rcv $@ $(LIBOBJS)
	ranlib $@

libbuddy.so: $(BUDDYOBJS) 
	$(LD) -shared -fPIC -o $@  $(BUDDYOBJS)

libbuddy.a: $(BUDDYOBJS)
	$(AR) rcv $@ $(BUDDYOBJS)
	ranlib $@

install: libs
	install --mode=644  List.h $(INCLUDE)/
	install --mode=644  Node.h $(INCLUDE)/
	install --mode=644  common.h $(INCLUDE)/
	install --mode=644  buddy.h $(INCLUDE)/
	install --mode=755  libmylib.so $(LIB)/libmylib.so
	install --mode=755  libmylib.a $(LIB)/libmylib.a
	install --mode=755  libbuddy.so $(LIB)/libbuddy.so
	install --mode=755  libbuddy.a $(LIB)/libbuddy.a

installclean:  clean
	rm --force $(LIB)/libmylib.so
	rm --force $(LIB)/libmylib.a
	rm --force $(LIB)/libbuddy.so
	rm --force $(LIB)/libbuddy.a
	rm --force $(INCLUDE)/List.h
	rm --force $(INCLUDE)/Node.h
	rm --force $(INCLUDE)/common.h
	rm --force $(INCLUDE)/buddy.h
	
clean:
	rm --force list *.o a.out core libmylib.a libmylib.so libbuddy.a libbuddy.so

