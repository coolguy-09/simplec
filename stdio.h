// stdio.h clone - SimpleC

#ifndef SIMPLEC_STDIO_H
#define SIMPLEC_STDIO_H

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
        : "eax", "ebx", "ecx", "edx"
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
        : "rax", "rdi", "rsi", "rdx"
    );
#else
    #error "Unsupported architecture for putchar."
#endif
}

static inline void printf(const char *s) {
    while (*s) putchar(*s++);
}

#endif // SIMPLEC_STDIO_H

