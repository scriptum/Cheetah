#!/bin/sh
DIR=./bin/Release
whereis i586-mingw32msvc-gcc | grep / > /dev/null
if [ $? = "1" ]
then
	echo "You must install mingw32 - Minimalist GNU win32 (cross) compiler"
	exit 1
fi
export CC=i586-mingw32msvc-gcc
make
mv $DIR/libcheetah.so $DIR/bin/win32/cheetah.dll