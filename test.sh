#!/bin/bash

if [ -z "$1" ]; then
    echo " Error : Invalid args !"
    echo " Exemple : ./test.sh my_test.c"
    exit 1
fi

EXEC_NAME="exec_`basename $1 .c`"

gcc "$1" -L. -lft_malloc -Wl,-rpath=. -o "$EXEC_NAME"

if [ $? -eq 0 ]; then
    echo -e "\n--- Compile $EXEC_NAME ---"
    ./"$EXEC_NAME"
else
    echo -e "\n--- Compilation Error ---"
fi
