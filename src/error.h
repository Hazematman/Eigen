#ifndef ERROR_H
#define ERROR_H

#define logError(m) logErrorMsg(__FILE__, __func__ ,__LINE__, (m)) 

void logErrorMsg(const char *file, const char *func, int line, const char *errorMsg);

#endif
