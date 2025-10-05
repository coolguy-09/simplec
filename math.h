// math.h clone - SimpleC

#ifndef SIMPLEC_MATH_H
#define SIMPLEC_MATH_H

static inline int pow(int base, int exp) {
    int result = 1;
    while (exp--) result *= base;
    return result;
}
static inline int abs(int x) { return x < 0 ? -x : x; }
static inline int max(int a, int b) { return a > b ? a : b; }
static inline int min(int a, int b) { return a < b ? a : b; }

static inline int clamp(int x, int lo, int hi) {
    return max(lo, min(x, hi));
}

static inline float sqrt(float x) {
    float result;
#if defined(__i386__)
    // Use x87 FPU instructions for 32-bit
    asm volatile (
        "flds %[input]\n"
        "fsqrt\n"
        "fstps %[output]\n"
        : [output] "=m" (result)
        : [input] "m" (x)
    );
#elif defined(__x86_64__)
    // Use SSE2 instructions for 64-bit
    asm volatile (
        "sqrtss %[input], %[output]\n"
        : [output] "=x" (result)
        : [input] "x" (x)
    );
#else
    #error "Unsupported architecture for sqrt."
#endif
    return result;
}

static inline double floor(double x) {
    double result;
#if defined(__x86_64__)
    asm volatile (
        "roundsd $1, %[input], %[output]\n" // round down
        : [output] "=x"(result)
        : [input] "x"(x)
    );
#elif defined(__i386__)
    asm volatile (
        "fldl %[input]\n"
        "frndint\n"
        "fstpl %[output]\n"
        : [output] "=m"(result)
        : [input] "m"(x)
    );
#else
    #error "Unsupported architecture for floor."
#endif
    return result;
}

static inline double ceil(double x) {
    double result;
#if defined(__x86_64__)
    asm volatile (
        "roundsd $2, %[input], %[output]\n" // round up
        : [output] "=x"(result)
        : [input] "x"(x)
    );
#elif defined(__i386__)
    asm volatile (
        "fldl %[input]\n"
        "fchs\n"
        "frndint\n"
        "fchs\n"
        "fstpl %[output]\n"
        : [output] "=m"(result)
        : [input] "m"(x)
    );
#else
    #error "Unsupported architecture for ceil."
#endif
    return result;
}

static inline double round(double x) {
    double result;
#if defined(__x86_64__)
    asm volatile (
        "roundsd $0, %[input], %[output]\n" // round to nearest
        : [output] "=x"(result)
        : [input] "x"(x)
    );
#elif defined(__i386__)
    asm volatile (
        "fldl %[input]\n"
        "fldl %[half]\n"
        "faddp %%st(1), %%st(0)\n"
        "frndint\n"
        "fstpl %[output]\n"
        : [output] "=m"(result)
        : [input] "m"(x), [half] "m"(0.5)
    );
#else
    #error "Unsupported architecture for round."
#endif
    return result;
}

static inline double sin(double x) {
    double x2 = x * x;
    double term = x;
    double result = term;

    term *= -x2 / (2 * 3);
    result += term;
    term *= -x2 / (4 * 5);
    result += term;
    term *= -x2 / (6 * 7);
    result += term;
    term *= -x2 / (8 * 9);
    result += term;

    return result;
}

static inline double cos(double x) {
    double x2 = x * x;
    double term = 1.0;
    double result = term;

    term *= -x2 / (1 * 2);
    result += term;
    term *= -x2 / (3 * 4);
    result += term;
    term *= -x2 / (5 * 6);
    result += term;
    term *= -x2 / (7 * 8);
    result += term;

    return result;
}

#endif // SIMPLEC_MATH_H
