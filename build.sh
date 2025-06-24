#!/usr/bin/env bash

# File paths
MAIN="src/main.c"
OUTPUT="build/Snake"

# Extra libs needed
LIBS="-lncurses"

# Menu options
if [ "$1" = "compile" ]; then 
	gcc -Wall -Wextra $MAIN $LIBS -o $OUTPUT
elif [ "$1" = "run" ]; then
	./$OUTPUT
fi