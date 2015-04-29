
CC=gcc
CFLAGS=-g -Wall
IFLAGS=-I../include -I.
LIBS=-lreadline -lncurses 
OBJECTS=mydash.o job.o git_version.o
CUSTOM_LIBS_DIR=-L../lib
CUSTOM_LIBS=-lmylib


BUILD_NUMBER_FILE=version-number.txt

PROGS=mydash 

all: $(PROGS) 

.c.o:
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

mydash: $(OBJECTS) 
	$(CC) $(CFLAGS) $(IFLAGS) -o ../$@  $(OBJECTS) $(LIBS) $(CUSTOM_LIBS_DIR) $(CUSTOM_LIBS)

git_version.c: FORCE
	echo '#ifndef __GIT_VERSION_H' > git_version.h
	echo '#define __GIT_VERSION_H' >> git_version.h
	echo 'const char* git_version(void);' >> git_version.h
	echo '#endif' >> git_version.h
	echo -n 'const char* git_version(void) { const char* GIT_Version = "' > git_version.c
	git rev-parse HEAD | cut -c1-4 | tr -d '\n'  >> git_version.c
	echo '"; return GIT_Version; }' >> git_version.c

FORCE:

clean:
	/bin/rm -f *.o $(PROGS) a.out core  *.log
	/bin/rm -fr docs
	/bin/rm -fr docs doxygen.log

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config >& doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

