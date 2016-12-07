#include <stdio.h>
#include <stdlib.h>
#include "utilTest/arrayTest.h"
#include "error.h"
#include "tassert.h"

void logErrorMsg(const char *file, const char *func, int line, const char *msg) {
    printf("Error in %s on line %d from %s:\n%s\n", func, line, file, msg);    
    exit(0);
}

void tassertFunc(const char *file, const char *func, int line, const char *condStr, bool cond) {
    if(cond == false) {
        logErrorMsg(file, func, line, condStr);
    }
}

int main() {
    printf("Eigen tests\n");

    printf("Running Array Tests\n");
    arrayTest();

    printf("Tests completed succefully!\n");
    return 0;
}
