// sys/types.h clone - SimpleC

#ifndef SIMPLEC_SYS_TYPES_H
#define SIMPLEC_SYS_TYPES_H

// Standard integer types
typedef unsigned char       u_char;
typedef unsigned short      u_short;
typedef unsigned int        u_int;
typedef unsigned long       u_long;
typedef unsigned long long  u_quad_t;
typedef long long           quad_t;

// Standard sizes
typedef unsigned long       size_t;
typedef long                ssize_t;
typedef long                ptrdiff_t;

// File system types
typedef unsigned int        dev_t;
typedef unsigned int        ino_t;
typedef unsigned int        mode_t;
typedef unsigned int        nlink_t;
typedef long                off_t;
typedef long                blksize_t;
typedef long                blkcnt_t;
typedef unsigned short      uid_t;
typedef unsigned short      gid_t;

// Time types
typedef long                time_t;
typedef long                suseconds_t;

// Process and signal types
typedef int                 pid_t;
typedef int                 id_t;
typedef int                 daddr_t;
typedef int                 key_t;
typedef int                 clock_t;
typedef int                 sig_atomic_t;

// 64-bit types for large files
typedef long long           off64_t;
typedef unsigned long long  ino64_t;
typedef long long           blkcnt64_t;
typedef long long           loff_t;

#endif // SIMPLEC_SYS_TYPES_H
