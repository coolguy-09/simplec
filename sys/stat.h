#ifndef SIMPLEC_SYS_STAT_H
#define SIMPLEC_SYS_STAT_H

typedef unsigned long   size_t;
typedef long            ssize_t;
typedef long            off_t;
typedef int             pid_t;
typedef int             uid_t;
typedef int             gid_t;
typedef int             mode_t;
typedef long            time_t;
typedef unsigned int    dev_t;
typedef unsigned int    ino_t;
typedef unsigned int    nlink_t;
typedef long            blksize_t;
typedef long            blkcnt_t;

// File type masks
#define S_IFMT   0170000 // bitmask for the file type bitfields
#define S_IFREG  0100000 // regular file
#define S_IFDIR  0040000 // directory
#define S_IFCHR  0020000 // character device
#define S_IFBLK  0060000 // block device
#define S_IFIFO  0010000 // FIFO
#define S_IFLNK  0120000 // symbolic link
#define S_IFSOCK 0140000 // socket

// File mode bits
#define S_ISUID  04000
#define S_ISGID  02000
#define S_ISVTX  01000

#define S_IRUSR  0400
#define S_IWUSR  0200
#define S_IXUSR  0100
#define S_IRGRP  0040
#define S_IWGRP  0020
#define S_IXGRP  0010
#define S_IROTH  0004
#define S_IWOTH  0002
#define S_IXOTH  0001

// Macros to test file type
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

// struct stat
struct stat {
    dev_t     st_dev;
    ino_t     st_ino;
    nlink_t   st_nlink;
    mode_t    st_mode;
    uid_t     st_uid;
    gid_t     st_gid;
    dev_t     st_rdev;
    off_t     st_size;
    blksize_t st_blksize;
    blkcnt_t  st_blocks;
    time_t    st_atime;
    time_t    st_mtime;
    time_t    st_ctime;
};

// stat syscall wrappers
#if defined(__i386__)
static inline int stat(const char *pathname, struct stat *st) {
    int ret;
    register unsigned long eax asm("eax") = 106; // sys_stat
    register unsigned long ebx asm("ebx") = (unsigned long)pathname;
    register unsigned long ecx asm("ecx") = (unsigned long)st;
    asm volatile("int $0x80"
                 : "+a"(eax)
                 : "b"(ebx), "c"(ecx)
                 : "memory");
    ret = eax;
    return ret;
}

#elif defined(__x86_64__)
static inline int stat(const char *pathname, struct stat *st) {
    long ret;
    asm volatile(
        "movq $4, %%rax\n"     // sys_newfstatat
        "movq $-100, %%rdi\n"  // AT_FDCWD
        "movq %[path], %%rsi\n"
        "movq %[stbuf], %%rdx\n"
        "movq $0, %%r10\n"     // flags
        "syscall\n"
        "movq %%rax, %[ret]"
        : [ret] "=r"(ret)
        : [path] "r"(pathname), [stbuf] "r"(st)
        : "rax","rdi","rsi","rdx","r10","rcx","r11","memory");
    return (int)ret;
}

#else
#error "Unsupported architecture for stat."
#endif

#endif // SIMPLEC_SYS_STAT_H

