#include "ntreeTest.h"
#include "util/ntree.h"
#include "../tassert.h"

static void emptyTree() {
    int val = 10;
    struct TreeNode *n = nodeCreate(&val, sizeof(int)); 

    tassert(val == *(int*)nodeGetElem(n));
    tassert(0 == nodeNumChild(n));

    nodeDestroy(n);
}

static void oneChild() {
    int val1 = 1, val2 = 2;
    struct TreeNode *n1 = nodeCreate(&val1, sizeof(int));
    struct TreeNode *n2 = nodeCreate(&val2, sizeof(int));

    nodeAddChild(n1, n2);

    tassert(val1 == *(int*)nodeGetElem(n1));
    tassert(1 == nodeNumChild(n1));
    tassert(n2 == nodeGetChild(n1, 0));

    tassert(val2 == *(int*)nodeGetElem(n2));
    tassert(0 == nodeNumChild(n2));

    nodeDestroy(n1);
}

void ntreeTest() {
    emptyTree();
    oneChild();
}
