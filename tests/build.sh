#!/bin/bash
SRCPATH="../src"
FILES="util/array.c"

TESTFILES="main.c utilTest/arrayTest.c"
for FILE in $FILES
do
    TESTFILES="$TESTFILES $SRCPATH/$FILE"
done

gcc -Wall -I../src $TESTFILES -o tests
