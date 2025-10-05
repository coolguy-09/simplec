# SimpleC

A super-lightweight libc clone.

## What is SimpleC?

SimpleC is a super-lightweight, fast-compiling, freestanding libc clone for C development, targeted to mostly devs looking for a libc alternative.

## Why SimpleC?
### 🪶 It's super-lightweight:
  SimpleC is super-lightweight. A simple program thats uses SimpleC with -static and -nostdlib is under 10KB!
  
### ✨ It's compiles super-fast:
  SimpleC is super-lightweight so it compiles super-fast because there is no fluff compared to libc.
  
### 🧠 It has libc-like behavior:
  SimpleC acts pretty much like libc. You probably can't see a difference.

## Using SimpleC

SimpleC is super-easy to import, just copy the .h file(s) to your current directory. Then just import them like this:
```C
#include "<filename>.h"
```


> [!WARNING]
> When using SimpleC with the flag -nostdlib, there is a bug that where if you **don't** exit after a function, it will give a segmentation fault.
> To avoid this, you can simply use the exit() function by including the stdlib header like this:
> ```C
> #include "stdlib.h"
>
> void _start() {
>   // Your code here...
>   exit(0);
> }
> ```

> [!TIP]
> When compiling, if you are getting a bunch of warnings like this:
> ```
> warning: conflicting types for built-in function ‘pow’; expected ‘double(double,  double)’ [-Wbuiltin-declaration-mismatch]
> ```
> You can use the -fno-builtin flag in GCC so you can avoid these warnings. Like this:
> ```bash
> gcc -static -fno-builtin example.c -o example
> ```
