#!/bin/bash
SRCPATH="../src"
FILES="util/array.c util/dfa.c util/ntree.c script/scanner.c script/cfg.c script/parser.c script/lr1.c"

TESTFILES="main.c utilTest/arrayTest.c utilTest/dfaTest.c utilTest/ntreeTest.c scriptTest/scannerTest.c scriptTest/parserTest.c"
for FILE in $FILES
do
    TESTFILES="$TESTFILES $SRCPATH/$FILE"
done

gcc -g -Wall -I../src $TESTFILES -o tests
