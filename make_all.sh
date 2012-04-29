#!/bin/sh
premake --clean
premake --target gnu
make clean
sh make_win.sh
make clean
sh make_linux.sh