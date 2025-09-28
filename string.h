// string.h clone - SimpleC

#ifndef SIMPLEC_STRING_H
#define SIMPLEC_STRING_H

// strlen: count characters until null terminator
static inline unsigned long strlen(const char *s) {
    unsigned long len = 0;
    while (*s++) len++;
    return len;
}

// strcpy: copy string from src to dest
static inline char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

// strcmp: compare two strings
static inline int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) a++, b++;
    return *(unsigned char *)a - *(unsigned char *)b;
}

// memcpy: copy n bytes from src to dest
static inline void *memcpy(void *dest, const void *src, unsigned long n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}

// memset: fill memory with a byte value
static inline void *memset(void *dest, int val, unsigned long n) {
    unsigned char *d = dest;
    while (n--) *d++ = (unsigned char)val;
    return dest;
}

#endif // SIMPLEC_STRING_H
