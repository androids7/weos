#include "string.h"
#include "assert.h"
void *memcpy(void *dest, const void *src, size_t count) {
	if (dest == NULL || src == NULL || count < 0)
		return NULL;
	char *tempDest = (char *)dest;
	char *tempSrc = (char *)src;
	while (count-- > 0)
		*tempDest++ = *tempSrc++;
	return dest;
}
void *memset(void *src, int chunk, size_t count) {
	if (src == NULL || count < 0)
		return NULL;
	char *tempSrc = (char *)src;
	while (count-- > 0)
		*tempSrc++ = chunk;
	return src;
}
void bzero(void *src, unsigned len) {
	register char *tempSrc = (char *)src;
	while (len != 0) {
		*tempSrc++ = 0;
		len--;
	}
}
char *strcpy(char *dest, const char *src) {
	char *addr = dest;
	assert((dest != NULL) && (src != NULL));
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return addr;
}
char *strcat(char *dest, const char *src) {
	assert(dest != NULL && src != NULL);
	char *ptr = dest;
	while (*ptr++)
		--ptr;
	while ((*ptr++ = *src++) != 0);
	return dest;
}
int strcmp(const char *str1, const char *str2) {
	assert((str1 != NULL) && (str2 != NULL));
	while (*str1 && *str2 && (*str1 == *str2)) {
		str1++;
		str2++;
	}
	return *str1 - *str2;
}
int strlen(const char *src) {
	if (src == NULL)
		return NULL;
	return ('\0' != *src) ? (1 + strlen(++src)) : 0;
}
