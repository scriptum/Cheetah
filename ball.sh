#!/bin/sh
REL_TYPE=release
make clean
sh build.sh native $REL_TYPE
make clean
sh build.sh win32 $REL_TYPE
make clean
sh build.sh linux32 $REL_TYPE
