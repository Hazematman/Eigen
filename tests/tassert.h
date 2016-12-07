#ifndef TASSERT_H
#define TASSERT_H
#include <stdbool.h>

#define tassert(cond) tassertFunc(__FILE__, __func__, __LINE__, #cond, (cond))

void tassertFunc(const char *file, const char *func, int line, const char *condStr, bool cond);

#endif
