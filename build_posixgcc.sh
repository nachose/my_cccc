#! /bin/sh
#  build_posixgcc.sh

# This file builds and tests CCCC on a POSIX platform using the
# GNU C++ compiler.

cd pccts 
make clean
make
cd ..

cd cccc
make -f posixgcc.mak clean
make -j 3 -f posixgcc.mak 
cd ..

cd test
make -j 3 -f posix.mak 
cd ..


