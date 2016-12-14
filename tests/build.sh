#!/bin/bash
SRCPATH="../src"
FILES="util/array.c util/dfa.c script/scanner.c"

TESTFILES="main.c utilTest/arrayTest.c utilTest/dfaTest.c scriptTest/scannerTest.c"
for FILE in $FILES
do
    TESTFILES="$TESTFILES $SRCPATH/$FILE"
done

gcc -g -Wall -I../src $TESTFILES -o tests
