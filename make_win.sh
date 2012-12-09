#!/bin/sh
DIR=./bin/Release
whereis i586-mingw32msvc-gcc | grep / > /dev/null
if [ $? = "1" ]
then
	whereis i586-pc-mingw32-gcc | grep / > /dev/null
	if [ $? = "1" ]
	then
		echo "You must install mingw32 - Minimalist GNU win32 (cross) compiler"
		exit 1
	else
		MINGWGCC=i586-pc-mingw32-gcc
	fi
else
	MINGWGCC=i586-mingw32msvc-gcc
fi

export CC=$MINGWGCC
if [ "debug" != "$1" ]
then
	export LDFLAGS=-s
else
	export CFLAGS=-g
fi
make && mv libcheetah.so $DIR/bin/win32/cheetah.dll