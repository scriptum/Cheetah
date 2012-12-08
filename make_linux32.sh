#!/bin/sh
DIR=./bin/Release
make clean
CFLAGS=-m32 make && mv libcheetah.so $DIR/bin/linux32/libcheetah.so