#ifndef STDIO_H
#define STDIO_H
#include "stddef.h"
#include "stdarg.h"
void clear();
void write(char *str);
void putchar(char c);
void printk(const char *format, ...);
#endif // STDIO_H
