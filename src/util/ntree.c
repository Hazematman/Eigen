#include <string.h>
#include "ntree.h"
#include "array.h"
#include "../error.h"

struct TreeNode {
    void *data;
    struct Array *nodes;
};

struct TreeNode *nodeCreate(void *elem, size_t eSize) {
    struct TreeNode *node = malloc(sizeof(struct TreeNode));
    if(node == NULL) {
        logError("Out of memory!");
    }

    node->data = malloc(eSize);
    if(node->data == NULL) {
        logError("Out of memory!");
    }

    memcpy(node->data, elem, eSize);

    node->nodes = arrayCreate(sizeof(struct TreeNode*));

    return node;
}

void nodeDestroy(struct TreeNode *node) {
    for(size_t i=0; i < arrayLength(node->nodes); i++) {
        struct TreeNode **n = (struct TreeNode **)arrayGet(node->nodes, i);
        nodeDestroy(*n);
    }

    arrayDestroy(node->nodes);
    free(node->data);
    free(node);
}

void *nodeGetElem(struct TreeNode *node) {
    return node->data;
}

void nodeAddChild(struct TreeNode *root, struct TreeNode *child) {
    arrayPush(root->nodes, &child);
}

size_t nodeNumChild(struct TreeNode *node) {
    return arrayLength(node->nodes);
}

struct TreeNode *nodeGetChild(struct TreeNode *node, size_t index) {
    struct TreeNode **n = arrayGet(node->nodes, index);
    return *n;
}
