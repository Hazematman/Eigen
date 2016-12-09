#include <stdio.h>
#include "arrayTest.h"
#include "util/array.h"
#include "error.h"
#include "../tassert.h"

struct Thing {
    int a;
    int b;
};

static struct Array *a;
static int testVal1 = 10;
static int testVal2 = 3;
static int testVal3 = 25;

static struct Array *b;
static struct Thing t1 = {1,2};
static struct Thing t2 = {3,4};

static void largeArray() {
    b = arrayCreate(sizeof(struct Thing));
    tassert(b != NULL);

    arrayPush(b, &t1);
    arrayPush(b, &t2);

    struct Thing val1 = *(struct Thing*)arrayGet(b, 0);
    struct Thing val2 = *(struct Thing*)arrayGet(b, 1);

    tassert(arrayLength(b) == 2);
    tassert(val1.a == t1.a);
    tassert(val1.b == t1.b);
    tassert(val2.a == t2.a);
    tassert(val2.b == t2.b);

    arrayDelete(b, 0);
    val1 = *(struct Thing*)arrayGet(b, 0);
    tassert(arrayLength(b) == 1);
    tassert(val1.a == t2.a);
    tassert(val1.b == t2.b);

    arrayDestroy(b);
}

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

    largeArray();
}
