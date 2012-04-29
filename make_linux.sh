#!/bin/sh
DIR=./bin/Release
make
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
mv $DIR/libcheetah.so $DIR/bin/linux$MACHINE_NAME/libcheetah.so