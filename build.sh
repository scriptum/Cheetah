#!/bin/sh
TMP=/tmp/cheetah-build-last
LAST=native
CORES=`nproc`
THREADS=`expr $CORES \* 2`
MAKE="/usr/bin/make -j$THREADS"
CLEAN="/usr/bin/make clean"
DIR=./bin/Release
MACHINE_NAME=`uname -m`
case ${MACHINE_NAME} in
i486|i586|i686)
  MACHINE_NAME=32
  ;;
x86_64)
  MACHINE_NAME=64
  ;;
*)
  MACHINE_NAME=
  ;;
esac

OS=linux
LIB=libcheetah.so
EXE=luajit
COMPILER=gcc
FLAGS_THIRDPARTY=""
if [ "$1" == "win" -o "$1" == "windows" ]
then
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
	COMPILER=$MINGWGCC
	OS=win
	MACHINE_NAME=32
	LIB=cheetah.dll
	EXE=luajit.exe
	LAST=win
fi

GCC_VERSION=$($COMPILER -v |& tail -1 | awk '{print $3}' | sed s/\\.//g)
FLAGS_OPTIMIZE_GENERAL="-fomit-frame-pointer -funroll-loops -mmmx -msse -mtune=generic"

if [ $GCC_VERSION -ge 460 ]
then
	FLAGS_OPTIMIZE="-Ofast $FLAGS_OPTIMIZE_GENERAL"
else
	FLAGS_OPTIMIZE="-O3 -ffast-math $FLAGS_OPTIMIZE_GENERAL"
fi

if [ "$2" == "release" -o "$2" == "final" ]
then
	#add extra flags for gcc
	FLAGS="$FLAGS_OPTIMIZE -Winline -Wdisabled-optimization"
else
	if [ "$2" == "debug" ]
	then
		FLAGS=-g
	else
		FLAGS=""
	fi
	#clang has faster compile time than gcc and produces good code w/o -O3
	which clang > /dev/null 2>&1
	if [ "$?" = "0" ]
	then
		COMPILER=clang
		echo "Using clang"
	fi
fi

#~ which ccache > /dev/null 2>&1
#~ if [ "$?" = "0" ]
#~ then
	#~ COMPILER="ccache $COMPILER"
	#~ echo "Using ccache"
#~ fi

export CC="$COMPILER"

if [ "$1" == "linux32" ]
then
	FLAGS="$FLAGS -m32"
	MACHINE_NAME=32
	FLAGS_THIRDPARTY="-m32"
	LAST=linux32
fi

LIBPATH="$DIR/bin/$OS$MACHINE_NAME/$LIB"
EXEPATH="./bin/$OS$MACHINE_NAME/$EXE"

if [ ! -f "$DIR/bin/$OS$MACHINE_NAME/$EXE" ]
then
	echo "Building luajit..."
	pushd .
	cd thirdparty/LuaJIT
	make clean
	if [ "$1" == "win" -o "$1" == "windows" ]
	then
		make HOST_CC="gcc -m32" CROSS=`echo $MINGWGCC | sed s/-gcc/-/` TARGET_SYS=Windows
		cp src/luajit.dll "../../$DIR/bin/$OS$MACHINE_NAME/"
	else
		if [ "$1" == "linux32" ]
		then
			CFLAGS="$CFLAGS -m32 make"
		else
			make
		fi
	fi
	cp src/$EXE "../../$DIR/bin/$OS$MACHINE_NAME/"
	popd
fi

if [ "$CFLAGS" ]
then FLAGS="$FLAGS $CFLAGS"
fi

echo "Building on $CORES cores with flags: $FLAGS"

if [ "`cat $TMP`" != "$LAST $2" ]
then
	$CLEAN
fi
echo "$LAST $2" > $TMP
if [ "$2" == "final" ]
then
	if [ $GCC_VERSION -ge 450 ]
	then
		FLAGS="$FLAGS -flto"
	fi
	$CLEAN
	CFLAGS="$FLAGS -fprofile-generate" $MAKE && mv libcheetah.so $LIBPATH
	pushd .
	cd $DIR
	pushd .
	cd Demos/Tests/LoadImages
	$EXEPATH main.lua
	popd
	pushd .
	cd Demos/Tests/Math
	$EXEPATH main.lua
	popd
	popd
	$CLEAN
	CFLAGS="$FLAGS -fprofile-use" $MAKE && mv libcheetah.so $LIBPATH
else
	CFLAGS="$FLAGS" $MAKE && mv libcheetah.so $LIBPATH
fi

if [ "$2" != "debug" ]
then
	strip -s $LIBPATH
fi

sh genheader.sh
