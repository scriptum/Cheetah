#!/bin/sh
make clean
sh make_win.sh
make clean
sh make_linux_32.sh
make clean
sh make_linux.sh