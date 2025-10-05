// stdio.h clone - SimpleC

#ifndef SIMPLEC_STDIO_H
#define SIMPLEC_STDIO_H

#include <stdarg.h>

// putchar using syscall (x86_64 + i386)
static inline void putchar(char c) {
#if defined(__i386__)
    asm volatile (
        "movl $4, %%eax\n"
        "movl $1, %%ebx\n"
        "movl %[buf], %%ecx\n"
        "movl $1, %%edx\n"
        "int $0x80\n"
        :
        : [buf] "r" (&c)
        : "eax","ebx","ecx","edx"
    );
#elif defined(__x86_64__)
    asm volatile (
        "movq $1, %%rax\n"
        "movq $1, %%rdi\n"
        "movq %[buf], %%rsi\n"
        "movq $1, %%rdx\n"
        "syscall\n"
        :
        : [buf] "r" (&c)
        : "rax","rdi","rsi","rdx"
    );
#else
    #error "Unsupported architecture for putchar."
#endif
}

static inline void io_itoa(int n, char *buf, unsigned long bufsize) {
    if (bufsize == 0) return;
    char tmp[12]; unsigned long i=0, ti=0;
    unsigned int x = (n < 0) ? -n : n;
    if (n==0) tmp[ti++]='0';
    else while(x && ti<sizeof(tmp)) { tmp[ti++]='0'+(x%10); x/=10; }
    if(n<0) tmp[ti++]='-';
    while(ti && i+1<bufsize) buf[i++] = tmp[--ti];
    buf[i] = '\0';
}

static inline void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    while(*fmt) {
        if(*fmt=='%') {
            fmt++;
            switch(*fmt) {
                case 'd': {
                    int val = va_arg(args, int);
                    char buf[12];
                    io_itoa(val, buf, sizeof(buf));
                    char *p = buf; while(*p) putchar(*p++);
                    break;
                }
                case 's': {
                    char *s = va_arg(args, char*);
                    while(*s) putchar(*s++);
                    break;
                }
                case 'c': {
                    int c = va_arg(args, int);
                    putchar((char)c);
                    break;
                }
                case '%': {
                    putchar('%');
                    break;
                }
                default: {
                    putchar('%'); putchar(*fmt);
                }
            }
        } else putchar(*fmt);
        fmt++;
    }
    va_end(args);
}

#endif // SIMPLEC_STDIO_H
