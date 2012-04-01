#!/bin/sh
whereis i586-mingw32msvc-gcc | grep / > /dev/null
if [ $? = "1" ]
then
	echo "You must install mingw32 - Minimalist GNU win32 (cross) compiler"
	exit 1
fi
export CC=i586-mingw32msvc-gcc
$CC -c *.c  -Os -fomit-frame-pointer -s -fPIC
$CC --shared -o SOIL.dll *.o -lGL -L../../lib
rm *.o