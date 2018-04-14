#ifndef STDARG_H
#define STDARG_H
#include "stddef.h"
typedef char* va_list;
#define va_start(p, first) (p = (va_list)&first + sizeof(first))
#define va_arg(p, next) (*(next*)((p += sizeof(next) ) - sizeof(next)))
#define va_end(p) (p = (va_list)NULL)
#endif // STDARG_H