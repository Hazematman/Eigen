#!/bin/bash
SRCPATH="../src"
FILES="util/array.c util/dfa.c"

TESTFILES="main.c utilTest/arrayTest.c utilTest/dfaTest.c"
for FILE in $FILES
do
    TESTFILES="$TESTFILES $SRCPATH/$FILE"
done

gcc -g -Wall -I../src $TESTFILES -o tests
