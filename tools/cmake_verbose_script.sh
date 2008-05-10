#!/bin/bash

INSTALL_PREFIX=/usr

cd ../
mkdir build
cd build/
cmake -DCMAKE_VERBOSE_MAKEFILE=ON \
	-DCMAKE_CXX_FLAGS="-Wall -Wextra -O2 -g3" \
	-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
	../
