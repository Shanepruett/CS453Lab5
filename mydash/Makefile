

all: libs dash testcode


libs: 
	cd libsrc; make install

dash:  libs
	cd dash; make

testcode: libs dash
	cd testsuite; make

clean:
	cd libsrc; make installclean
	cd testsuite; make clean
	cd dash; make clean
	rm -f mydash



