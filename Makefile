

SHELL = /bin/sh



all:
	perl -MExtUtils::Embed -e xsinit -- -o perlxsi.c
	gcc -c perlxsi.c `perl -MExtUtils::Embed -e ccopts`
	gcc -c perlnecklace.c  `perl -MExtUtils::Embed -e ccopts`
	gcc  -Wall -Wpointer-arith -Wstrict-prototypes -o perlnecklace perlxsi.o perlnecklace.o `perl -MExtUtils::Embed -e ldopts` 
	chmod 4755 perlnecklace

.PHONY: clean clean-test clean-all


clean:
	-rm perlnecklace
	-rm perlnecklace.o
	-rm perlxsi*
	-rm testfile_* 	

clean-test:
	-rm testfile_*

clean-all:
	clean
	clean-test

test-openfiles:
	./perlnecklace -w test/openfiles.pl


