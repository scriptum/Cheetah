#!/bin/sh
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
if [ "debug" != "$1" ]
then
	export LDFLAGS=-s
else
	export CFLAGS=-g
fi
make && mv libcheetah.so $DIR/bin/linux$MACHINE_NAME/libcheetah.so
