#ifndef NTREE_H
#define NTREE_H
#include <stdlib.h>

struct TreeNode;

struct TreeNode *nodeCreate(void *elem, size_t eSize);
void nodeDestroy(struct TreeNode *node);
void *nodeGetElem(struct TreeNode *node);
void nodeAddChild(struct TreeNode *root, struct TreeNode *child);
size_t nodeNumChild(struct TreeNode *node);
struct TreeNode *nodeGetChild(struct TreeNode *node, size_t index);
#endif
