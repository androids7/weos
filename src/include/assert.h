#ifndef ASSERT_H
#define ASSERT_H
#include "stdio.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdelf.h"
#ifndef NDEBUG
#define assert(expr) (void)((expr) || ((_assert(#expr,__FILE__,__LINE__),0)))
#else
#define assert(expr) ((void)0)
#endif // NDEBUG
void _assert(void *exp, void *file, unsigned int line);
void init_debug();
void panic(const char *msg);
void print_stack_trace();
void print_cur_status();
#endif // ASSERT_H
