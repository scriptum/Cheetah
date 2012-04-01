#!/bin/sh
gcc -c *.c -Os -fomit-frame-pointer -s -fPIC
gcc --shared -o libSOIL.so *.o -lGL
rm *.o