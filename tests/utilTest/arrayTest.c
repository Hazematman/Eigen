#include <stdio.h>
#include "arrayTest.h"
#include "util/array.h"
#include "error.h"
#include "../tassert.h"

static struct Array *a;
static int testVal1 = 10;
static int testVal2 = 3;
static int testVal3 = 25;

static void testCreateArray() {
    a = arrayCreate(sizeof(int));
    tassert(a != NULL);
}

static void testAdding() {
    int val = testVal1;

    tassert(arrayLength(a) == 0);

    arrayPush(a, &val);

    tassert(arrayLength(a) == 1);
    tassert(*(int*)arrayGet(a, 0) == val);

    int val2 = testVal2;

    arrayPush(a, &val2);
    arrayPush(a, &val);

    tassert(arrayLength(a) == 3);
    tassert(*(int*)arrayGet(a, 0) == val);
    tassert(*(int*)arrayGet(a, 1) == val2);
    tassert(*(int*)arrayGet(a, 2) == val);
}

static void testPop() {
    int val1 = 0, val2 = 0;

    arrayPop(a, &val1);

    tassert(arrayLength(a) == 2);
    tassert(val1 == testVal1);

    arrayPop(a, &val2);

    tassert(arrayLength(a) == 1);
    tassert(val2 == testVal2);

    arrayPop(a, &val1);

    tassert(arrayLength(a) == 0);
    tassert(val1 == testVal1);
}

static void testDelete() {
    int val1 = testVal1;
    int val2 = testVal2;
    int val3 = testVal3;

    arrayPush(a, &val1);
    arrayPush(a, &val2);
    arrayPush(a, &val3);

    tassert(arrayLength(a) == 3);
    tassert(*(int*)arrayGet(a, 0) == val1);
    tassert(*(int*)arrayGet(a, 1) == val2);
    tassert(*(int*)arrayGet(a, 2) == val3);

    arrayDelete(a, 0);
    
    tassert(arrayLength(a) == 2);
    tassert(*(int*)arrayGet(a, 0) == val2);
    tassert(*(int*)arrayGet(a, 1) == val3);

    arrayDelete(a, 1);
    tassert(arrayLength(a) == 1);
    tassert(*(int*)arrayGet(a, 0) == val2);

    arrayDelete(a, 0);
    tassert(arrayLength(a) == 0);
}

static void testCleanup() {
    arrayDestroy(a);
}

void arrayTest() {
    testCreateArray();
    testAdding();
    testPop();
    testDelete();
    testCleanup();
}
