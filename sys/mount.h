// sys/mount.h clone - SimpleC

#ifndef SIMPLEC_SYS_MOUNT_H
#define SIMPLEC_SYS_MOUNT_H

// Mount flags
#define MS_RDONLY       1
#define MS_NOSUID       2
#define MS_NODEV        4
#define MS_NOEXEC       8
#define MS_SYNCHRONOUS  16
#define MS_REMOUNT      32
#define MS_MANDLOCK     64
#define MS_DIRSYNC      128
#define MS_NOATIME      1024
#define MS_NODIRATIME   2048
#define MS_BIND         4096

// Umount flags
#define MNT_FORCE       1
#define MNT_DETACH      2
#define MNT_EXPIRE      4
#define UMOUNT_NOFOLLOW 8

// Syscall wrappers
#if defined(__i386__)

// i386 mount syscall using register variables
static inline int mount(const char *source, const char *target,
                        const char *fstype, unsigned long flags,
                        const void *data) {
    int ret;
    register unsigned long eax asm("eax") = 21;
    register unsigned long ebx asm("ebx") = (unsigned long)source;
    register unsigned long ecx asm("ecx") = (unsigned long)target;
    register unsigned long edx asm("edx") = (unsigned long)fstype;
    register unsigned long esi asm("esi") = flags;
    register unsigned long edi asm("edi") = (unsigned long)data;

    asm volatile("int $0x80"
                 : "+a"(eax)
                 : "b"(ebx), "c"(ecx), "d"(edx), "S"(esi), "D"(edi)
                 : "memory");
    ret = eax;
    return ret;
}

// i386 umount2 syscall
static inline int umount(const char *target, int flags) {
    int ret;
    register unsigned long eax asm("eax") = 52;
    register unsigned long ebx asm("ebx") = (unsigned long)target;
    register unsigned long ecx asm("ecx") = (unsigned long)flags;

    asm volatile("int $0x80"
                 : "+a"(eax)
                 : "b"(ebx), "c"(ecx)
                 : "memory");
    ret = eax;
    return ret;
}

#elif defined(__x86_64__)

// x86_64 mount syscall
static inline int mount(const char *source, const char *target,
                        const char *fstype, unsigned long flags,
                        const void *data) {
    long ret;
    asm volatile (
        "movq $165, %%rax\n"
        "movq %[src], %%rdi\n"
        "movq %[tgt], %%rsi\n"
        "movq %[fst], %%rdx\n"
        "movq %[flg], %%r10\n"
        "movq %[dat], %%r8\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [src] "r"(source),
          [tgt] "r"(target),
          [fst] "r"(fstype),
          [flg] "r"(flags),
          [dat] "r"(data)
        : "rax","rdi","rsi","rdx","r10","r8","rcx","r11","memory"
    );
    return (int)ret;
}

// x86_64 umount2 syscall
static inline int umount(const char *target, int flags) {
    long ret;
    asm volatile (
        "movq $166, %%rax\n"
        "movq %[tgt], %%rdi\n"
        "movq %[flg], %%rsi\n"
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [tgt] "r"(target),
          [flg] "r"((long)flags)
        : "rax","rdi","rsi","rcx","r11","memory"
    );
    return (int)ret;
}

#else
#error "Unsupported architecture for mount/umount."
#endif

#endif // SIMPLEC_SYS_MOUNT_H

