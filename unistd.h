// unistd.h clone - SimpleC

#ifndef SIMPLEC_UNISTD_H
#define SIMPLEC_UNISTD_H

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

typedef long ssize_t;
typedef unsigned long size_t;
typedef long off_t;
typedef int pid_t;

// Syscall wrappers
#if defined(__i386__)

// waitpid(pid_t, status, options)
static inline pid_t waitpid(pid_t pid, int *status, int options) {
    long ret;
    register unsigned long eax asm("eax") = 7; // sys_waitpid
    register unsigned long ebx asm("ebx") = pid;
    register unsigned long ecx asm("ecx") = (unsigned long)status;
    register unsigned long edx asm("edx") = options;
    asm volatile("int $0x80" : "+a"(eax) : "b"(ebx), "c"(ecx), "d"(edx) : "memory");
    ret = eax;
    return (ret < 0) ? -ret : ret;
}

// read(fd, buf, count)
static inline ssize_t read(int fd, void *buf, size_t count) {
    long ret;
    register unsigned long eax asm("eax") = 3;
    register unsigned long ebx asm("ebx") = fd;
    register unsigned long ecx asm("ecx") = (unsigned long)buf;
    register unsigned long edx asm("edx") = count;
    asm volatile("int $0x80" : "+a"(eax) : "b"(ebx), "c"(ecx), "d"(edx) : "memory");
    ret = eax;
    return (ret < 0) ? -ret : ret;
}

// write(fd, buf, count)
static inline ssize_t write(int fd, const void *buf, size_t count) {
    long ret;
    register unsigned long eax asm("eax") = 4;
    register unsigned long ebx asm("ebx") = fd;
    register unsigned long ecx asm("ecx") = (unsigned long)buf;
    register unsigned long edx asm("edx") = count;
    asm volatile("int $0x80" : "+a"(eax) : "b"(ebx), "c"(ecx), "d"(edx) : "memory");
    ret = eax;
    return (ret < 0) ? -ret : ret;
}

// getpid()
static inline pid_t getpid(void) {
    long ret;
    register unsigned long eax asm("eax") = 20;
    asm volatile("int $0x80" : "+a"(eax) : : "memory");
    ret = eax;
    return (ret < 0) ? -ret : ret;
}

// fork()
static inline pid_t fork(void) {
    long ret;
    register unsigned long eax asm("eax") = 2;
    asm volatile("int $0x80" : "+a"(eax) : : "memory");
    ret = eax;
    return (ret < 0) ? -ret : ret;
}

// execve(path, argv, envp)
static inline int execve(const char *path, char *const argv[], char *const envp[]) {
    long ret;
    register unsigned long eax asm("eax") = 11;
    register unsigned long ebx asm("ebx") = (unsigned long)path;
    register unsigned long ecx asm("ecx") = (unsigned long)argv;
    register unsigned long edx asm("edx") = (unsigned long)envp;
    asm volatile("int $0x80" : "+a"(eax) : "b"(ebx), "c"(ecx), "d"(edx) : "memory");
    ret = eax;
    return (ret < 0) ? -ret : ret;
}

// exit(status)
static inline void _exit(int status) {
    register unsigned long eax asm("eax") = 1;
    register unsigned long ebx asm("ebx") = status;
    asm volatile("int $0x80" : : "a"(eax), "b"(ebx) : "memory");
    while(1);
}

#elif defined(__x86_64__)

static inline pid_t waitpid(pid_t pid, int *status, int options) {
    long ret;
    asm volatile(
        "movq $61, %%rax\n"
        "movq %[pid], %%rdi\n"
        "movq %[stat], %%rsi\n"
        "movq %[opt], %%rdx\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [pid] "r"((long)pid), [stat] "r"(status), [opt] "r"((long)options)
        : "rax","rdi","rsi","rdx","rcx","r11","memory"
    );
    return (ret < 0) ? -ret : ret;
}

// read(fd, buf, count)
static inline ssize_t read(int fd, void *buf, size_t count) {
    long ret;
    asm volatile(
        "movq $0, %%rax\n"
        "movq %[fd], %%rdi\n"
        "movq %[buf], %%rsi\n"
        "movq %[cnt], %%rdx\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [fd] "r"((long)fd), [buf] "r"(buf), [cnt] "r"(count)
        : "rax","rdi","rsi","rdx","rcx","r11","memory"
    );
    return (ret < 0) ? -ret : ret;
}

// write(fd, buf, count)
static inline ssize_t write(int fd, const void *buf, size_t count) {
    long ret;
    asm volatile(
        "movq $1, %%rax\n"
        "movq %[fd], %%rdi\n"
        "movq %[buf], %%rsi\n"
        "movq %[cnt], %%rdx\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [fd] "r"((long)fd), [buf] "r"(buf), [cnt] "r"(count)
        : "rax","rdi","rsi","rdx","rcx","r11","memory"
    );
    return (ret < 0) ? -ret : ret;
}

// getpid()
static inline pid_t getpid(void) {
    long ret;
    asm volatile(
        "movq $39, %%rax\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        :
        : "rax","rdi","rcx","r11","memory"
    );
    return (ret < 0) ? -ret : ret;
}

// fork()
static inline pid_t fork(void) {
    long ret;
    asm volatile(
        "movq $57, %%rax\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        :
        : "rax","rdi","rcx","r11","memory"
    );
    return (ret < 0) ? -ret : ret;
}

// execve(path, argv, envp)
static inline int execve(const char *path, char *const argv[], char *const envp[]) {
    long ret;
    asm volatile(
        "movq $59, %%rax\n"
        "movq %[path], %%rdi\n"
        "movq %[argv], %%rsi\n"
        "movq %[envp], %%rdx\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [path] "r"(path), [argv] "r"(argv), [envp] "r"(envp)
        : "rax","rdi","rsi","rdx","rcx","r11","memory"
    );
    return (ret < 0) ? -ret : ret;
}

// exit(status)
static inline void _exit(int status) {
    asm volatile(
        "movq $60, %%rax\n"
        "movq %[st], %%rdi\n"
        "syscall"
        :
        : [st] "r"((long)status)
        : "rax","rdi","rcx","r11","memory"
    );
    while(1);
}

#else
#error "Unsupported architecture for unistd."
#endif

#endif // SIMPLEC_UNISTD_H
