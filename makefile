# Top level makefile for CCCC

# This distribution is a compilation of code, some of which comes from
# different sources, some of which builds different (e.g. Win32 DLL) kinds
# of targets.
# I would like to make it less inconsistent, but the first stage is to make 
# it work...

.PHONY : pccts cccc test clean install 

all : pccts cccc test install

pccts :
	cd pccts ; make

cccc : 
	cd cccc ; make -f posixgcc.mak

test :
	cd test ; make -f posix.mak

clean :
	cd cccc ; rm *.o ; rm cccc

install : 
	cd install ; sudo make -f install.mak 

