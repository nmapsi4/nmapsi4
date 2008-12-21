#!/bin/bash

INSTALL_PREFIX=/usr

cd ../
mkdir build
cd build/
cmake -DCMAKE_VERBOSE_MAKEFILE=ON \
	-DCMAKE_SKIP_RPATH=ON \
	-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
	../
