#!/bin/sh
DIR=./bin/Release
if [ "debug" != "$1" ]
then
	export LDFLAGS=-s
	export CFLAGS=-m32
else
	export CFLAGS="-g -m32"
fi
make && mv libcheetah.so $DIR/bin/linux32/libcheetah.so