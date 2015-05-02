

all: dox libs dash testcode


libs: 
	cd libsrc; make install

dash:  libs
	cd dash; make

testcode: libs dash
	cd testsuite; make
	cd buddy-test; make

dox:	
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config >> doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

clean:
	cd libsrc; make installclean
	cd testsuite; make clean
	cd dash; make clean
	cd buddy-test; make clean
	rm -f mydash
	/bin/rm -fr docs doxygen.log




