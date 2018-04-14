#ifndef STRING_H
#define STRING_H
#include "stddef.h"
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *src, int chunk, size_t count);
void bzero(void *dest, unsigned len);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(const char *src);
#endif // STRING_H
