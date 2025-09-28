// stdlib.h clone - SimpleC

#ifndef SIMPLEC_STDLIB_H
#define SIMPLEC_STDLIB_H

static inline void exit(int code) {
    asm volatile (
        "movl $1, %%eax\n"      // sys_exit
        "movl %[status], %%ebx\n"
        "int $0x80\n"
        :
        : [status] "r" (code)
        : "eax", "ebx"
    );
}

static inline int atoi(const char *s) {
    int result = 0;
    int sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        s++;
    }
    return sign * result;
}

static inline char *itoa(int value, char *buf) {
    char *p = buf;
    int is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }
    char *start = p;
    do {
        *p++ = '0' + (value % 10);
        value /= 10;
    } while (value);
    if (is_negative) *p++ = '-';
    *p = '\0';

    // Reverse the string
    for (char *a = start, *b = p - 1; a < b; a++, b--) {
        char t = *a;
        *a = *b;
        *b = t;
    }
    return buf;
}

static inline long strtol(const char *s, char **endptr, int base) {
    long result = 0;
    int sign = 1;

    while (*s == ' ' || *s == '\t') s++; // skip whitespace
    if (*s == '-') { sign = -1; s++; }
    else if (*s == '+') { s++; }

    if (base == 0) {
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            base = 16;
            s += 2;
        } else if (s[0] == '0') {
            base = 8;
            s++;
        } else {
            base = 10;
        }
    }

    while (*s) {
        int digit;
        if (*s >= '0' && *s <= '9') digit = *s - '0';
        else if (*s >= 'a' && *s <= 'f') digit = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'F') digit = *s - 'A' + 10;
        else break;

        if (digit >= base) break;

        result = result * base + digit;
        s++;
    }

    if (endptr) *endptr = (char *)s;
    return sign * result;
}

static inline void ftoa(double value, char *buf, int precision) {
    if (value < 0) {
        *buf++ = '-';
        value = -value;
    }

    int int_part = (int)value;
    double frac_part = value - int_part;

    // Convert integer part
    char temp[16];
    char *p = temp;
    if (int_part == 0) *p++ = '0';
    else {
        while (int_part) {
            *p++ = '0' + (int_part % 10);
            int_part /= 10;
        }
    }
    while (p != temp) *buf++ = *--p;

    *buf++ = '.';

    // Convert fractional part
    for (int i = 0; i < precision; i++) {
        frac_part *= 10;
        int digit = (int)frac_part;
        *buf++ = '0' + digit;
        frac_part -= digit;
    }

    *buf = '\0';
}

#endif
