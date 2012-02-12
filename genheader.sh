#!/bin/sh
#header generator from *.c files
FILENAME=./bin/Release/lib/cheetah.h
cat src/cheetah.h | awk 'BEGIN {x=0}{if ($0~"typedef struct"||$0~"enum"||$0~"struct") {x=1}if (x==1) {print $0}if ($0~"}") {x=0}}' > $FILENAME
grep -h "^[a-zA-Z].*(.*) {\s*$" src/*.c | sed "s/ {/;/" >> $FILENAME