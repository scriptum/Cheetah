void cpMessage(const char *condition, const char *file, int line, int isError, int isHardError, const char *message, ...);


















typedef long double float_t;

typedef long double double_t;


extern double acos (double __x) __attribute__ ((__nothrow__)); extern double __acos (double __x) __attribute__ ((__nothrow__));

extern double asin (double __x) __attribute__ ((__nothrow__)); extern double __asin (double __x) __attribute__ ((__nothrow__));

extern double atan (double __x) __attribute__ ((__nothrow__)); extern double __atan (double __x) __attribute__ ((__nothrow__));

extern double atan2 (double __y, double __x) __attribute__ ((__nothrow__)); extern double __atan2 (double __y, double __x) __attribute__ ((__nothrow__));


extern double cos (double __x) __attribute__ ((__nothrow__)); extern double __cos (double __x) __attribute__ ((__nothrow__));

extern double sin (double __x) __attribute__ ((__nothrow__)); extern double __sin (double __x) __attribute__ ((__nothrow__));

extern double tan (double __x) __attribute__ ((__nothrow__)); extern double __tan (double __x) __attribute__ ((__nothrow__));




extern double cosh (double __x) __attribute__ ((__nothrow__)); extern double __cosh (double __x) __attribute__ ((__nothrow__));

extern double sinh (double __x) __attribute__ ((__nothrow__)); extern double __sinh (double __x) __attribute__ ((__nothrow__));

extern double tanh (double __x) __attribute__ ((__nothrow__)); extern double __tanh (double __x) __attribute__ ((__nothrow__));



extern double acosh (double __x) __attribute__ ((__nothrow__)); extern double __acosh (double __x) __attribute__ ((__nothrow__));

extern double asinh (double __x) __attribute__ ((__nothrow__)); extern double __asinh (double __x) __attribute__ ((__nothrow__));

extern double atanh (double __x) __attribute__ ((__nothrow__)); extern double __atanh (double __x) __attribute__ ((__nothrow__));







extern double exp (double __x) __attribute__ ((__nothrow__)); extern double __exp (double __x) __attribute__ ((__nothrow__));


extern double frexp (double __x, int *__exponent) __attribute__ ((__nothrow__)); extern double __frexp (double __x, int *__exponent) __attribute__ ((__nothrow__));


extern double ldexp (double __x, int __exponent) __attribute__ ((__nothrow__)); extern double __ldexp (double __x, int __exponent) __attribute__ ((__nothrow__));


extern double log (double __x) __attribute__ ((__nothrow__)); extern double __log (double __x) __attribute__ ((__nothrow__));


extern double log10 (double __x) __attribute__ ((__nothrow__)); extern double __log10 (double __x) __attribute__ ((__nothrow__));


extern double modf (double __x, double *__iptr) __attribute__ ((__nothrow__)); extern double __modf (double __x, double *__iptr) __attribute__ ((__nothrow__));



extern double expm1 (double __x) __attribute__ ((__nothrow__)); extern double __expm1 (double __x) __attribute__ ((__nothrow__));


extern double log1p (double __x) __attribute__ ((__nothrow__)); extern double __log1p (double __x) __attribute__ ((__nothrow__));


extern double logb (double __x) __attribute__ ((__nothrow__)); extern double __logb (double __x) __attribute__ ((__nothrow__));






extern double exp2 (double __x) __attribute__ ((__nothrow__)); extern double __exp2 (double __x) __attribute__ ((__nothrow__));


extern double log2 (double __x) __attribute__ ((__nothrow__)); extern double __log2 (double __x) __attribute__ ((__nothrow__));








extern double pow (double __x, double __y) __attribute__ ((__nothrow__)); extern double __pow (double __x, double __y) __attribute__ ((__nothrow__));


extern double sqrt (double __x) __attribute__ ((__nothrow__)); extern double __sqrt (double __x) __attribute__ ((__nothrow__));





extern double hypot (double __x, double __y) __attribute__ ((__nothrow__)); extern double __hypot (double __x, double __y) __attribute__ ((__nothrow__));






extern double cbrt (double __x) __attribute__ ((__nothrow__)); extern double __cbrt (double __x) __attribute__ ((__nothrow__));








extern double ceil (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __ceil (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern double fabs (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __fabs (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern double floor (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __floor (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern double fmod (double __x, double __y) __attribute__ ((__nothrow__)); extern double __fmod (double __x, double __y) __attribute__ ((__nothrow__));




extern int __isinf (double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern int __finite (double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));





extern int isinf (double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern int finite (double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern double drem (double __x, double __y) __attribute__ ((__nothrow__)); extern double __drem (double __x, double __y) __attribute__ ((__nothrow__));



extern double significand (double __x) __attribute__ ((__nothrow__)); extern double __significand (double __x) __attribute__ ((__nothrow__));





extern double copysign (double __x, double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __copysign (double __x, double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));






extern double nan (__const char *__tagb) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __nan (__const char *__tagb) __attribute__ ((__nothrow__)) __attribute__ ((__const__));





extern int __isnan (double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern int isnan (double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern double j0 (double) __attribute__ ((__nothrow__)); extern double __j0 (double) __attribute__ ((__nothrow__));
extern double j1 (double) __attribute__ ((__nothrow__)); extern double __j1 (double) __attribute__ ((__nothrow__));
extern double jn (int, double) __attribute__ ((__nothrow__)); extern double __jn (int, double) __attribute__ ((__nothrow__));
extern double y0 (double) __attribute__ ((__nothrow__)); extern double __y0 (double) __attribute__ ((__nothrow__));
extern double y1 (double) __attribute__ ((__nothrow__)); extern double __y1 (double) __attribute__ ((__nothrow__));
extern double yn (int, double) __attribute__ ((__nothrow__)); extern double __yn (int, double) __attribute__ ((__nothrow__));






extern double erf (double) __attribute__ ((__nothrow__)); extern double __erf (double) __attribute__ ((__nothrow__));
extern double erfc (double) __attribute__ ((__nothrow__)); extern double __erfc (double) __attribute__ ((__nothrow__));
extern double lgamma (double) __attribute__ ((__nothrow__)); extern double __lgamma (double) __attribute__ ((__nothrow__));






extern double tgamma (double) __attribute__ ((__nothrow__)); extern double __tgamma (double) __attribute__ ((__nothrow__));





extern double gamma (double) __attribute__ ((__nothrow__)); extern double __gamma (double) __attribute__ ((__nothrow__));






extern double lgamma_r (double, int *__signgamp) __attribute__ ((__nothrow__)); extern double __lgamma_r (double, int *__signgamp) __attribute__ ((__nothrow__));







extern double rint (double __x) __attribute__ ((__nothrow__)); extern double __rint (double __x) __attribute__ ((__nothrow__));


extern double nextafter (double __x, double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __nextafter (double __x, double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));

extern double nexttoward (double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __nexttoward (double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern double remainder (double __x, double __y) __attribute__ ((__nothrow__)); extern double __remainder (double __x, double __y) __attribute__ ((__nothrow__));



extern double scalbn (double __x, int __n) __attribute__ ((__nothrow__)); extern double __scalbn (double __x, int __n) __attribute__ ((__nothrow__));



extern int ilogb (double __x) __attribute__ ((__nothrow__)); extern int __ilogb (double __x) __attribute__ ((__nothrow__));




extern double scalbln (double __x, long int __n) __attribute__ ((__nothrow__)); extern double __scalbln (double __x, long int __n) __attribute__ ((__nothrow__));



extern double nearbyint (double __x) __attribute__ ((__nothrow__)); extern double __nearbyint (double __x) __attribute__ ((__nothrow__));



extern double round (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __round (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern double trunc (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern double __trunc (double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));




extern double remquo (double __x, double __y, int *__quo) __attribute__ ((__nothrow__)); extern double __remquo (double __x, double __y, int *__quo) __attribute__ ((__nothrow__));






extern long int lrint (double __x) __attribute__ ((__nothrow__)); extern long int __lrint (double __x) __attribute__ ((__nothrow__));
extern long long int llrint (double __x) __attribute__ ((__nothrow__)); extern long long int __llrint (double __x) __attribute__ ((__nothrow__));



extern long int lround (double __x) __attribute__ ((__nothrow__)); extern long int __lround (double __x) __attribute__ ((__nothrow__));
extern long long int llround (double __x) __attribute__ ((__nothrow__)); extern long long int __llround (double __x) __attribute__ ((__nothrow__));



extern double fdim (double __x, double __y) __attribute__ ((__nothrow__)); extern double __fdim (double __x, double __y) __attribute__ ((__nothrow__));


extern double fmax (double __x, double __y) __attribute__ ((__nothrow__)); extern double __fmax (double __x, double __y) __attribute__ ((__nothrow__));


extern double fmin (double __x, double __y) __attribute__ ((__nothrow__)); extern double __fmin (double __x, double __y) __attribute__ ((__nothrow__));



extern int __fpclassify (double __value) __attribute__ ((__nothrow__))
     __attribute__ ((__const__));


extern int __signbit (double __value) __attribute__ ((__nothrow__))
     __attribute__ ((__const__));



extern double fma (double __x, double __y, double __z) __attribute__ ((__nothrow__)); extern double __fma (double __x, double __y, double __z) __attribute__ ((__nothrow__));








extern double scalb (double __x, double __n) __attribute__ ((__nothrow__)); extern double __scalb (double __x, double __n) __attribute__ ((__nothrow__));


extern float acosf (float __x) __attribute__ ((__nothrow__)); extern float __acosf (float __x) __attribute__ ((__nothrow__));

extern float asinf (float __x) __attribute__ ((__nothrow__)); extern float __asinf (float __x) __attribute__ ((__nothrow__));

extern float atanf (float __x) __attribute__ ((__nothrow__)); extern float __atanf (float __x) __attribute__ ((__nothrow__));

extern float atan2f (float __y, float __x) __attribute__ ((__nothrow__)); extern float __atan2f (float __y, float __x) __attribute__ ((__nothrow__));


extern float cosf (float __x) __attribute__ ((__nothrow__)); extern float __cosf (float __x) __attribute__ ((__nothrow__));

extern float sinf (float __x) __attribute__ ((__nothrow__)); extern float __sinf (float __x) __attribute__ ((__nothrow__));

extern float tanf (float __x) __attribute__ ((__nothrow__)); extern float __tanf (float __x) __attribute__ ((__nothrow__));




extern float coshf (float __x) __attribute__ ((__nothrow__)); extern float __coshf (float __x) __attribute__ ((__nothrow__));

extern float sinhf (float __x) __attribute__ ((__nothrow__)); extern float __sinhf (float __x) __attribute__ ((__nothrow__));

extern float tanhf (float __x) __attribute__ ((__nothrow__)); extern float __tanhf (float __x) __attribute__ ((__nothrow__));



extern float acoshf (float __x) __attribute__ ((__nothrow__)); extern float __acoshf (float __x) __attribute__ ((__nothrow__));

extern float asinhf (float __x) __attribute__ ((__nothrow__)); extern float __asinhf (float __x) __attribute__ ((__nothrow__));

extern float atanhf (float __x) __attribute__ ((__nothrow__)); extern float __atanhf (float __x) __attribute__ ((__nothrow__));







extern float expf (float __x) __attribute__ ((__nothrow__)); extern float __expf (float __x) __attribute__ ((__nothrow__));


extern float frexpf (float __x, int *__exponent) __attribute__ ((__nothrow__)); extern float __frexpf (float __x, int *__exponent) __attribute__ ((__nothrow__));


extern float ldexpf (float __x, int __exponent) __attribute__ ((__nothrow__)); extern float __ldexpf (float __x, int __exponent) __attribute__ ((__nothrow__));


extern float logf (float __x) __attribute__ ((__nothrow__)); extern float __logf (float __x) __attribute__ ((__nothrow__));


extern float log10f (float __x) __attribute__ ((__nothrow__)); extern float __log10f (float __x) __attribute__ ((__nothrow__));


extern float modff (float __x, float *__iptr) __attribute__ ((__nothrow__)); extern float __modff (float __x, float *__iptr) __attribute__ ((__nothrow__));



extern float expm1f (float __x) __attribute__ ((__nothrow__)); extern float __expm1f (float __x) __attribute__ ((__nothrow__));


extern float log1pf (float __x) __attribute__ ((__nothrow__)); extern float __log1pf (float __x) __attribute__ ((__nothrow__));


extern float logbf (float __x) __attribute__ ((__nothrow__)); extern float __logbf (float __x) __attribute__ ((__nothrow__));






extern float exp2f (float __x) __attribute__ ((__nothrow__)); extern float __exp2f (float __x) __attribute__ ((__nothrow__));


extern float log2f (float __x) __attribute__ ((__nothrow__)); extern float __log2f (float __x) __attribute__ ((__nothrow__));








extern float powf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __powf (float __x, float __y) __attribute__ ((__nothrow__));


extern float sqrtf (float __x) __attribute__ ((__nothrow__)); extern float __sqrtf (float __x) __attribute__ ((__nothrow__));





extern float hypotf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __hypotf (float __x, float __y) __attribute__ ((__nothrow__));






extern float cbrtf (float __x) __attribute__ ((__nothrow__)); extern float __cbrtf (float __x) __attribute__ ((__nothrow__));








extern float ceilf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __ceilf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern float fabsf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __fabsf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern float floorf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __floorf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern float fmodf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __fmodf (float __x, float __y) __attribute__ ((__nothrow__));




extern int __isinff (float __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern int __finitef (float __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));





extern int isinff (float __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern int finitef (float __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern float dremf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __dremf (float __x, float __y) __attribute__ ((__nothrow__));



extern float significandf (float __x) __attribute__ ((__nothrow__)); extern float __significandf (float __x) __attribute__ ((__nothrow__));





extern float copysignf (float __x, float __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __copysignf (float __x, float __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));






extern float nanf (__const char *__tagb) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __nanf (__const char *__tagb) __attribute__ ((__nothrow__)) __attribute__ ((__const__));





extern int __isnanf (float __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern int isnanf (float __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern float j0f (float) __attribute__ ((__nothrow__)); extern float __j0f (float) __attribute__ ((__nothrow__));
extern float j1f (float) __attribute__ ((__nothrow__)); extern float __j1f (float) __attribute__ ((__nothrow__));
extern float jnf (int, float) __attribute__ ((__nothrow__)); extern float __jnf (int, float) __attribute__ ((__nothrow__));
extern float y0f (float) __attribute__ ((__nothrow__)); extern float __y0f (float) __attribute__ ((__nothrow__));
extern float y1f (float) __attribute__ ((__nothrow__)); extern float __y1f (float) __attribute__ ((__nothrow__));
extern float ynf (int, float) __attribute__ ((__nothrow__)); extern float __ynf (int, float) __attribute__ ((__nothrow__));






extern float erff (float) __attribute__ ((__nothrow__)); extern float __erff (float) __attribute__ ((__nothrow__));
extern float erfcf (float) __attribute__ ((__nothrow__)); extern float __erfcf (float) __attribute__ ((__nothrow__));
extern float lgammaf (float) __attribute__ ((__nothrow__)); extern float __lgammaf (float) __attribute__ ((__nothrow__));






extern float tgammaf (float) __attribute__ ((__nothrow__)); extern float __tgammaf (float) __attribute__ ((__nothrow__));





extern float gammaf (float) __attribute__ ((__nothrow__)); extern float __gammaf (float) __attribute__ ((__nothrow__));






extern float lgammaf_r (float, int *__signgamp) __attribute__ ((__nothrow__)); extern float __lgammaf_r (float, int *__signgamp) __attribute__ ((__nothrow__));







extern float rintf (float __x) __attribute__ ((__nothrow__)); extern float __rintf (float __x) __attribute__ ((__nothrow__));


extern float nextafterf (float __x, float __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __nextafterf (float __x, float __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));

extern float nexttowardf (float __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __nexttowardf (float __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern float remainderf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __remainderf (float __x, float __y) __attribute__ ((__nothrow__));



extern float scalbnf (float __x, int __n) __attribute__ ((__nothrow__)); extern float __scalbnf (float __x, int __n) __attribute__ ((__nothrow__));



extern int ilogbf (float __x) __attribute__ ((__nothrow__)); extern int __ilogbf (float __x) __attribute__ ((__nothrow__));




extern float scalblnf (float __x, long int __n) __attribute__ ((__nothrow__)); extern float __scalblnf (float __x, long int __n) __attribute__ ((__nothrow__));



extern float nearbyintf (float __x) __attribute__ ((__nothrow__)); extern float __nearbyintf (float __x) __attribute__ ((__nothrow__));



extern float roundf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __roundf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern float truncf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern float __truncf (float __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));




extern float remquof (float __x, float __y, int *__quo) __attribute__ ((__nothrow__)); extern float __remquof (float __x, float __y, int *__quo) __attribute__ ((__nothrow__));






extern long int lrintf (float __x) __attribute__ ((__nothrow__)); extern long int __lrintf (float __x) __attribute__ ((__nothrow__));
extern long long int llrintf (float __x) __attribute__ ((__nothrow__)); extern long long int __llrintf (float __x) __attribute__ ((__nothrow__));



extern long int lroundf (float __x) __attribute__ ((__nothrow__)); extern long int __lroundf (float __x) __attribute__ ((__nothrow__));
extern long long int llroundf (float __x) __attribute__ ((__nothrow__)); extern long long int __llroundf (float __x) __attribute__ ((__nothrow__));



extern float fdimf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __fdimf (float __x, float __y) __attribute__ ((__nothrow__));


extern float fmaxf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __fmaxf (float __x, float __y) __attribute__ ((__nothrow__));


extern float fminf (float __x, float __y) __attribute__ ((__nothrow__)); extern float __fminf (float __x, float __y) __attribute__ ((__nothrow__));



extern int __fpclassifyf (float __value) __attribute__ ((__nothrow__))
     __attribute__ ((__const__));


extern int __signbitf (float __value) __attribute__ ((__nothrow__))
     __attribute__ ((__const__));



extern float fmaf (float __x, float __y, float __z) __attribute__ ((__nothrow__)); extern float __fmaf (float __x, float __y, float __z) __attribute__ ((__nothrow__));








extern float scalbf (float __x, float __n) __attribute__ ((__nothrow__)); extern float __scalbf (float __x, float __n) __attribute__ ((__nothrow__));


extern long double acosl (long double __x) __attribute__ ((__nothrow__)); extern long double __acosl (long double __x) __attribute__ ((__nothrow__));

extern long double asinl (long double __x) __attribute__ ((__nothrow__)); extern long double __asinl (long double __x) __attribute__ ((__nothrow__));

extern long double atanl (long double __x) __attribute__ ((__nothrow__)); extern long double __atanl (long double __x) __attribute__ ((__nothrow__));

extern long double atan2l (long double __y, long double __x) __attribute__ ((__nothrow__)); extern long double __atan2l (long double __y, long double __x) __attribute__ ((__nothrow__));


extern long double cosl (long double __x) __attribute__ ((__nothrow__)); extern long double __cosl (long double __x) __attribute__ ((__nothrow__));

extern long double sinl (long double __x) __attribute__ ((__nothrow__)); extern long double __sinl (long double __x) __attribute__ ((__nothrow__));

extern long double tanl (long double __x) __attribute__ ((__nothrow__)); extern long double __tanl (long double __x) __attribute__ ((__nothrow__));




extern long double coshl (long double __x) __attribute__ ((__nothrow__)); extern long double __coshl (long double __x) __attribute__ ((__nothrow__));

extern long double sinhl (long double __x) __attribute__ ((__nothrow__)); extern long double __sinhl (long double __x) __attribute__ ((__nothrow__));

extern long double tanhl (long double __x) __attribute__ ((__nothrow__)); extern long double __tanhl (long double __x) __attribute__ ((__nothrow__));



extern long double acoshl (long double __x) __attribute__ ((__nothrow__)); extern long double __acoshl (long double __x) __attribute__ ((__nothrow__));

extern long double asinhl (long double __x) __attribute__ ((__nothrow__)); extern long double __asinhl (long double __x) __attribute__ ((__nothrow__));

extern long double atanhl (long double __x) __attribute__ ((__nothrow__)); extern long double __atanhl (long double __x) __attribute__ ((__nothrow__));







extern long double expl (long double __x) __attribute__ ((__nothrow__)); extern long double __expl (long double __x) __attribute__ ((__nothrow__));


extern long double frexpl (long double __x, int *__exponent) __attribute__ ((__nothrow__)); extern long double __frexpl (long double __x, int *__exponent) __attribute__ ((__nothrow__));


extern long double ldexpl (long double __x, int __exponent) __attribute__ ((__nothrow__)); extern long double __ldexpl (long double __x, int __exponent) __attribute__ ((__nothrow__));


extern long double logl (long double __x) __attribute__ ((__nothrow__)); extern long double __logl (long double __x) __attribute__ ((__nothrow__));


extern long double log10l (long double __x) __attribute__ ((__nothrow__)); extern long double __log10l (long double __x) __attribute__ ((__nothrow__));


extern long double modfl (long double __x, long double *__iptr) __attribute__ ((__nothrow__)); extern long double __modfl (long double __x, long double *__iptr) __attribute__ ((__nothrow__));



extern long double expm1l (long double __x) __attribute__ ((__nothrow__)); extern long double __expm1l (long double __x) __attribute__ ((__nothrow__));


extern long double log1pl (long double __x) __attribute__ ((__nothrow__)); extern long double __log1pl (long double __x) __attribute__ ((__nothrow__));


extern long double logbl (long double __x) __attribute__ ((__nothrow__)); extern long double __logbl (long double __x) __attribute__ ((__nothrow__));






extern long double exp2l (long double __x) __attribute__ ((__nothrow__)); extern long double __exp2l (long double __x) __attribute__ ((__nothrow__));


extern long double log2l (long double __x) __attribute__ ((__nothrow__)); extern long double __log2l (long double __x) __attribute__ ((__nothrow__));








extern long double powl (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __powl (long double __x, long double __y) __attribute__ ((__nothrow__));


extern long double sqrtl (long double __x) __attribute__ ((__nothrow__)); extern long double __sqrtl (long double __x) __attribute__ ((__nothrow__));





extern long double hypotl (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __hypotl (long double __x, long double __y) __attribute__ ((__nothrow__));






extern long double cbrtl (long double __x) __attribute__ ((__nothrow__)); extern long double __cbrtl (long double __x) __attribute__ ((__nothrow__));








extern long double ceill (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __ceill (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern long double fabsl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __fabsl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern long double floorl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __floorl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern long double fmodl (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __fmodl (long double __x, long double __y) __attribute__ ((__nothrow__));




extern int __isinfl (long double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern int __finitel (long double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));





extern int isinfl (long double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern int finitel (long double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern long double dreml (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __dreml (long double __x, long double __y) __attribute__ ((__nothrow__));



extern long double significandl (long double __x) __attribute__ ((__nothrow__)); extern long double __significandl (long double __x) __attribute__ ((__nothrow__));





extern long double copysignl (long double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __copysignl (long double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));






extern long double nanl (__const char *__tagb) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __nanl (__const char *__tagb) __attribute__ ((__nothrow__)) __attribute__ ((__const__));





extern int __isnanl (long double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern int isnanl (long double __value) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern long double j0l (long double) __attribute__ ((__nothrow__)); extern long double __j0l (long double) __attribute__ ((__nothrow__));
extern long double j1l (long double) __attribute__ ((__nothrow__)); extern long double __j1l (long double) __attribute__ ((__nothrow__));
extern long double jnl (int, long double) __attribute__ ((__nothrow__)); extern long double __jnl (int, long double) __attribute__ ((__nothrow__));
extern long double y0l (long double) __attribute__ ((__nothrow__)); extern long double __y0l (long double) __attribute__ ((__nothrow__));
extern long double y1l (long double) __attribute__ ((__nothrow__)); extern long double __y1l (long double) __attribute__ ((__nothrow__));
extern long double ynl (int, long double) __attribute__ ((__nothrow__)); extern long double __ynl (int, long double) __attribute__ ((__nothrow__));






extern long double erfl (long double) __attribute__ ((__nothrow__)); extern long double __erfl (long double) __attribute__ ((__nothrow__));
extern long double erfcl (long double) __attribute__ ((__nothrow__)); extern long double __erfcl (long double) __attribute__ ((__nothrow__));
extern long double lgammal (long double) __attribute__ ((__nothrow__)); extern long double __lgammal (long double) __attribute__ ((__nothrow__));






extern long double tgammal (long double) __attribute__ ((__nothrow__)); extern long double __tgammal (long double) __attribute__ ((__nothrow__));





extern long double gammal (long double) __attribute__ ((__nothrow__)); extern long double __gammal (long double) __attribute__ ((__nothrow__));






extern long double lgammal_r (long double, int *__signgamp) __attribute__ ((__nothrow__)); extern long double __lgammal_r (long double, int *__signgamp) __attribute__ ((__nothrow__));







extern long double rintl (long double __x) __attribute__ ((__nothrow__)); extern long double __rintl (long double __x) __attribute__ ((__nothrow__));


extern long double nextafterl (long double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __nextafterl (long double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));

extern long double nexttowardl (long double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __nexttowardl (long double __x, long double __y) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern long double remainderl (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __remainderl (long double __x, long double __y) __attribute__ ((__nothrow__));



extern long double scalbnl (long double __x, int __n) __attribute__ ((__nothrow__)); extern long double __scalbnl (long double __x, int __n) __attribute__ ((__nothrow__));



extern int ilogbl (long double __x) __attribute__ ((__nothrow__)); extern int __ilogbl (long double __x) __attribute__ ((__nothrow__));




extern long double scalblnl (long double __x, long int __n) __attribute__ ((__nothrow__)); extern long double __scalblnl (long double __x, long int __n) __attribute__ ((__nothrow__));



extern long double nearbyintl (long double __x) __attribute__ ((__nothrow__)); extern long double __nearbyintl (long double __x) __attribute__ ((__nothrow__));



extern long double roundl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __roundl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));



extern long double truncl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)); extern long double __truncl (long double __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__));




extern long double remquol (long double __x, long double __y, int *__quo) __attribute__ ((__nothrow__)); extern long double __remquol (long double __x, long double __y, int *__quo) __attribute__ ((__nothrow__));






extern long int lrintl (long double __x) __attribute__ ((__nothrow__)); extern long int __lrintl (long double __x) __attribute__ ((__nothrow__));
extern long long int llrintl (long double __x) __attribute__ ((__nothrow__)); extern long long int __llrintl (long double __x) __attribute__ ((__nothrow__));



extern long int lroundl (long double __x) __attribute__ ((__nothrow__)); extern long int __lroundl (long double __x) __attribute__ ((__nothrow__));
extern long long int llroundl (long double __x) __attribute__ ((__nothrow__)); extern long long int __llroundl (long double __x) __attribute__ ((__nothrow__));



extern long double fdiml (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __fdiml (long double __x, long double __y) __attribute__ ((__nothrow__));


extern long double fmaxl (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __fmaxl (long double __x, long double __y) __attribute__ ((__nothrow__));


extern long double fminl (long double __x, long double __y) __attribute__ ((__nothrow__)); extern long double __fminl (long double __x, long double __y) __attribute__ ((__nothrow__));



extern int __fpclassifyl (long double __value) __attribute__ ((__nothrow__))
     __attribute__ ((__const__));


extern int __signbitl (long double __value) __attribute__ ((__nothrow__))
     __attribute__ ((__const__));



extern long double fmal (long double __x, long double __y, long double __z) __attribute__ ((__nothrow__)); extern long double __fmal (long double __x, long double __y, long double __z) __attribute__ ((__nothrow__));








extern long double scalbl (long double __x, long double __n) __attribute__ ((__nothrow__)); extern long double __scalbl (long double __x, long double __n) __attribute__ ((__nothrow__));
extern int signgam;
enum
  {
    FP_NAN,

    FP_INFINITE,

    FP_ZERO,

    FP_SUBNORMAL,

    FP_NORMAL

  };
typedef enum
{
  _IEEE_ = -1,
  _SVID_,
  _XOPEN_,
  _POSIX_,
  _ISOC_
} _LIB_VERSION_TYPE;




extern _LIB_VERSION_TYPE _LIB_VERSION;
struct exception

  {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
  };




extern int matherr (struct exception *__exc);



typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;



__extension__
typedef long long int int64_t;




typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef unsigned int uint32_t;





__extension__
typedef unsigned long long int uint64_t;






typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;



__extension__
typedef long long int int_least64_t;



typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;



__extension__
typedef unsigned long long int uint_least64_t;






typedef signed char int_fast8_t;





typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;



typedef unsigned char uint_fast8_t;





typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;
typedef int intptr_t;


typedef unsigned int uintptr_t;
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
 typedef double cpFloat;
cpFloat cpfmax(cpFloat a, cpFloat b)
{
 return (a > b) ? a : b;
}


cpFloat cpfmin(cpFloat a, cpFloat b)
{
 return (a < b) ? a : b;
}


cpFloat cpfabs(cpFloat f)
{
 return (f < 0) ? -f : f;
}


cpFloat cpfclamp(cpFloat f, cpFloat min, cpFloat max)
{
 return cpfmin(cpfmax(f, min), max);
}


cpFloat cpfclamp01(cpFloat f)
{
 return cpfmax(0.0f, cpfmin(f, 1.0f));
}




cpFloat cpflerp(cpFloat f1, cpFloat f2, cpFloat t)
{
 return f1*(1.0f - t) + f2*t;
}


cpFloat cpflerpconst(cpFloat f1, cpFloat f2, cpFloat d)
{
 return f1 + cpfclamp(f2 - f1, -d, d);
}


typedef uintptr_t cpHashValue;






 typedef int cpBool;
 typedef void * cpDataPointer;






 typedef uintptr_t cpCollisionType;






 typedef uintptr_t cpGroup;






 typedef unsigned int cpLayers;






 typedef unsigned int cpTimestamp;
 typedef struct cpVect{cpFloat x,y;} cpVect;
typedef struct cpArray cpArray;
typedef struct cpHashSet cpHashSet;

typedef struct cpBody cpBody;
typedef struct cpShape cpShape;
typedef struct cpConstraint cpConstraint;

typedef struct cpCollisionHandler cpCollisionHandler;
typedef struct cpArbiter cpArbiter;

typedef struct cpSpace cpSpace;

static const cpVect cpvzero = {0.0f,0.0f};


cpVect cpv(const cpFloat x, const cpFloat y)
{
 cpVect v = {x, y};
 return v;
}


cpFloat cpvlength(const cpVect v);


cpVect cpvslerp(const cpVect v1, const cpVect v2, const cpFloat t);


cpVect cpvslerpconst(const cpVect v1, const cpVect v2, const cpFloat a);


cpVect cpvforangle(const cpFloat a);


cpFloat cpvtoangle(const cpVect v);




char* cpvstr(const cpVect v);


cpBool cpveql(const cpVect v1, const cpVect v2)
{
 return (v1.x == v2.x && v1.y == v2.y);
}


cpVect cpvadd(const cpVect v1, const cpVect v2)
{
 return cpv(v1.x + v2.x, v1.y + v2.y);
}


cpVect cpvsub(const cpVect v1, const cpVect v2)
{
 return cpv(v1.x - v2.x, v1.y - v2.y);
}


cpVect cpvneg(const cpVect v)
{
 return cpv(-v.x, -v.y);
}


cpVect cpvmult(const cpVect v, const cpFloat s)
{
 return cpv(v.x*s, v.y*s);
}


cpFloat cpvdot(const cpVect v1, const cpVect v2)
{
 return v1.x*v2.x + v1.y*v2.y;
}




cpFloat cpvcross(const cpVect v1, const cpVect v2)
{
 return v1.x*v2.y - v1.y*v2.x;
}


cpVect cpvperp(const cpVect v)
{
 return cpv(-v.y, v.x);
}


cpVect cpvrperp(const cpVect v)
{
 return cpv(v.y, -v.x);
}


cpVect cpvproject(const cpVect v1, const cpVect v2)
{
 return cpvmult(v2, cpvdot(v1, v2)/cpvdot(v2, v2));
}


cpVect cpvrotate(const cpVect v1, const cpVect v2)
{
 return cpv(v1.x*v2.x - v1.y*v2.y, v1.x*v2.y + v1.y*v2.x);
}


cpVect cpvunrotate(const cpVect v1, const cpVect v2)
{
 return cpv(v1.x*v2.x + v1.y*v2.y, v1.y*v2.x - v1.x*v2.y);
}


cpFloat cpvlengthsq(const cpVect v)
{
 return cpvdot(v, v);
}


cpVect cpvlerp(const cpVect v1, const cpVect v2, const cpFloat t)
{
 return cpvadd(cpvmult(v1, 1.0f - t), cpvmult(v2, t));
}


cpVect cpvnormalize(const cpVect v)
{
 return cpvmult(v, 1.0f/cpvlength(v));
}


cpVect cpvnormalize_safe(const cpVect v)
{
 return (v.x == 0.0f && v.y == 0.0f ? cpvzero : cpvnormalize(v));
}


cpVect cpvclamp(const cpVect v, const cpFloat len)
{
 return (cpvdot(v,v) > len*len) ? cpvmult(cpvnormalize(v), len) : v;
}


cpVect cpvlerpconst(cpVect v1, cpVect v2, cpFloat d)
{
 return cpvadd(v1, cpvclamp(cpvsub(v2, v1), d));
}


cpFloat cpvdist(const cpVect v1, const cpVect v2)
{
 return cpvlength(cpvsub(v1, v2));
}


cpFloat cpvdistsq(const cpVect v1, const cpVect v2)
{
 return cpvlengthsq(cpvsub(v1, v2));
}


cpBool cpvnear(const cpVect v1, const cpVect v2, const cpFloat dist)
{
 return cpvdistsq(v1, v2) < dist*dist;
}
typedef struct cpBB{
 cpFloat l, b, r ,t;
} cpBB;


cpBB cpBBNew(const cpFloat l, const cpFloat b, const cpFloat r, const cpFloat t)
{
 cpBB bb = {l, b, r, t};
 return bb;
}


cpBB cpBBNewForCircle(const cpVect p, const cpFloat r)
{
 return cpBBNew(p.x - r, p.y - r, p.x + r, p.y + r);
}


cpBool cpBBIntersects(const cpBB a, const cpBB b)
{
 return (a.l <= b.r && b.l <= a.r && a.b <= b.t && b.b <= a.t);
}


cpBool cpBBContainsBB(const cpBB bb, const cpBB other)
{
 return (bb.l <= other.l && bb.r >= other.r && bb.b <= other.b && bb.t >= other.t);
}


cpBool cpBBContainsVect(const cpBB bb, const cpVect v)
{
 return (bb.l <= v.x && bb.r >= v.x && bb.b <= v.y && bb.t >= v.y);
}


cpBB cpBBMerge(const cpBB a, const cpBB b){
 return cpBBNew(
  cpfmin(a.l, b.l),
  cpfmin(a.b, b.b),
  cpfmax(a.r, b.r),
  cpfmax(a.t, b.t)
 );
}


cpBB cpBBExpand(const cpBB bb, const cpVect v){
 return cpBBNew(
  cpfmin(bb.l, v.x),
  cpfmin(bb.b, v.y),
  cpfmax(bb.r, v.x),
  cpfmax(bb.t, v.y)
 );
}


cpFloat cpBBArea(cpBB bb)
{
 return (bb.r - bb.l)*(bb.t - bb.b);
}


cpFloat cpBBMergedArea(cpBB a, cpBB b)
{
 return (cpfmax(a.r, b.r) - cpfmin(a.l, b.l))*(cpfmax(a.t, b.t) - cpfmin(a.b, b.b));
}


cpFloat cpBBSegmentQuery(cpBB bb, cpVect a, cpVect b)
{
 cpFloat idx = 1.0f/(b.x - a.x);
 cpFloat tx1 = (bb.l == a.x ? -(__builtin_inff()) : (bb.l - a.x)*idx);
 cpFloat tx2 = (bb.r == a.x ? (__builtin_inff()) : (bb.r - a.x)*idx);
 cpFloat txmin = cpfmin(tx1, tx2);
 cpFloat txmax = cpfmax(tx1, tx2);

 cpFloat idy = 1.0f/(b.y - a.y);
 cpFloat ty1 = (bb.b == a.y ? -(__builtin_inff()) : (bb.b - a.y)*idy);
 cpFloat ty2 = (bb.t == a.y ? (__builtin_inff()) : (bb.t - a.y)*idy);
 cpFloat tymin = cpfmin(ty1, ty2);
 cpFloat tymax = cpfmax(ty1, ty2);

 if(tymin <= txmax && txmin <= tymax){
  cpFloat min = cpfmax(txmin, tymin);
  cpFloat max = cpfmin(txmax, tymax);

  if(0.0 <= max && min <= 1.0) return cpfmax(min, 0.0);
 }

 return (__builtin_inff());
}


cpBool cpBBIntersectsSegment(cpBB bb, cpVect a, cpVect b)
{
 return (cpBBSegmentQuery(bb, a, b) != (__builtin_inff()));
}


cpVect cpBBClampVect(const cpBB bb, const cpVect v);



cpVect cpBBWrapVect(const cpBB bb, const cpVect v);
typedef cpBB (*cpSpatialIndexBBFunc)(void *obj);

typedef void (*cpSpatialIndexIteratorFunc)(void *obj, void *data);

typedef void (*cpSpatialIndexQueryFunc)(void *obj1, void *obj2, void *data);

typedef cpFloat (*cpSpatialIndexSegmentQueryFunc)(void *obj1, void *obj2, void *data);


typedef struct cpSpatialIndexClass cpSpatialIndexClass;
typedef struct cpSpatialIndex cpSpatialIndex;


struct cpSpatialIndex {
 cpSpatialIndexClass *klass;

 cpSpatialIndexBBFunc bbfunc;

 cpSpatialIndex *staticIndex, *dynamicIndex;
};




typedef struct cpSpaceHash cpSpaceHash;


cpSpaceHash* cpSpaceHashAlloc(void);

cpSpatialIndex* cpSpaceHashInit(cpSpaceHash *hash, cpFloat celldim, int numcells, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex);

cpSpatialIndex* cpSpaceHashNew(cpFloat celldim, int cells, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex);





void cpSpaceHashResize(cpSpaceHash *hash, cpFloat celldim, int numcells);



typedef struct cpBBTree cpBBTree;


cpBBTree* cpBBTreeAlloc(void);

cpSpatialIndex* cpBBTreeInit(cpBBTree *tree, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex);

cpSpatialIndex* cpBBTreeNew(cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex);


void cpBBTreeOptimize(cpSpatialIndex *index);



typedef cpVect (*cpBBTreeVelocityFunc)(void *obj);

void cpBBTreeSetVelocityFunc(cpSpatialIndex *index, cpBBTreeVelocityFunc func);



typedef struct cpSweep1D cpSweep1D;


cpSweep1D* cpSweep1DAlloc(void);

cpSpatialIndex* cpSweep1DInit(cpSweep1D *sweep, cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex);

cpSpatialIndex* cpSweep1DNew(cpSpatialIndexBBFunc bbfunc, cpSpatialIndex *staticIndex);



typedef void (*cpSpatialIndexDestroyImpl)(cpSpatialIndex *index);

typedef int (*cpSpatialIndexCountImpl)(cpSpatialIndex *index);
typedef void (*cpSpatialIndexEachImpl)(cpSpatialIndex *index, cpSpatialIndexIteratorFunc func, void *data);

typedef cpBool (*cpSpatialIndexContainsImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexInsertImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexRemoveImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);

typedef void (*cpSpatialIndexReindexImpl)(cpSpatialIndex *index);
typedef void (*cpSpatialIndexReindexObjectImpl)(cpSpatialIndex *index, void *obj, cpHashValue hashid);
typedef void (*cpSpatialIndexReindexQueryImpl)(cpSpatialIndex *index, cpSpatialIndexQueryFunc func, void *data);

typedef void (*cpSpatialIndexPointQueryImpl)(cpSpatialIndex *index, cpVect point, cpSpatialIndexQueryFunc func, void *data);
typedef void (*cpSpatialIndexSegmentQueryImpl)(cpSpatialIndex *index, void *obj, cpVect a, cpVect b, cpFloat t_exit, cpSpatialIndexSegmentQueryFunc func, void *data);
typedef void (*cpSpatialIndexQueryImpl)(cpSpatialIndex *index, void *obj, cpBB bb, cpSpatialIndexQueryFunc func, void *data);

struct cpSpatialIndexClass {
 cpSpatialIndexDestroyImpl destroy;

 cpSpatialIndexCountImpl count;
 cpSpatialIndexEachImpl each;

 cpSpatialIndexContainsImpl contains;
 cpSpatialIndexInsertImpl insert;
 cpSpatialIndexRemoveImpl remove;

 cpSpatialIndexReindexImpl reindex;
 cpSpatialIndexReindexObjectImpl reindexObject;
 cpSpatialIndexReindexQueryImpl reindexQuery;

 cpSpatialIndexPointQueryImpl pointQuery;
 cpSpatialIndexSegmentQueryImpl segmentQuery;
 cpSpatialIndexQueryImpl query;
};


void cpSpatialIndexFree(cpSpatialIndex *index);

void cpSpatialIndexCollideStatic(cpSpatialIndex *dynamicIndex, cpSpatialIndex *staticIndex, cpSpatialIndexQueryFunc func, void *data);


void cpSpatialIndexDestroy(cpSpatialIndex *index)
{
 if(index->klass) index->klass->destroy(index);
}


int cpSpatialIndexCount(cpSpatialIndex *index)
{
 return index->klass->count(index);
}


void cpSpatialIndexEach(cpSpatialIndex *index, cpSpatialIndexIteratorFunc func, void *data)
{
 index->klass->each(index, func, data);
}



cpBool cpSpatialIndexContains(cpSpatialIndex *index, void *obj, cpHashValue hashid)
{
 return index->klass->contains(index, obj, hashid);
}



void cpSpatialIndexInsert(cpSpatialIndex *index, void *obj, cpHashValue hashid)
{
 index->klass->insert(index, obj, hashid);
}



void cpSpatialIndexRemove(cpSpatialIndex *index, void *obj, cpHashValue hashid)
{
 index->klass->remove(index, obj, hashid);
}


void cpSpatialIndexReindex(cpSpatialIndex *index)
{
 index->klass->reindex(index);
}


void cpSpatialIndexReindexObject(cpSpatialIndex *index, void *obj, cpHashValue hashid)
{
 index->klass->reindexObject(index, obj, hashid);
}



void cpSpatialIndexPointQuery(cpSpatialIndex *index, cpVect point, cpSpatialIndexQueryFunc func, void *data)
{
 index->klass->pointQuery(index, point, func, data);
}


void cpSpatialIndexSegmentQuery(cpSpatialIndex *index, void *obj, cpVect a, cpVect b, cpFloat t_exit, cpSpatialIndexSegmentQueryFunc func, void *data)
{
 index->klass->segmentQuery(index, obj, a, b, t_exit, func, data);
}


void cpSpatialIndexQuery(cpSpatialIndex *index, void *obj, cpBB bb, cpSpatialIndexQueryFunc func, void *data)
{
 index->klass->query(index, obj, bb, func, data);
}




void cpSpatialIndexReindexQuery(cpSpatialIndex *index, cpSpatialIndexQueryFunc func, void *data)
{
 index->klass->reindexQuery(index, func, data);
}

typedef void (*cpBodyVelocityFunc)(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);

typedef void (*cpBodyPositionFunc)(cpBody *body, cpFloat dt);



typedef struct cpComponentNode {
 cpBody *root;
 cpBody *next;
 cpFloat idleTime;
} cpComponentNode;


struct cpBody {

 cpBodyVelocityFunc velocity_func;


 cpBodyPositionFunc position_func;



 cpFloat m;

 cpFloat m_inv;



 cpFloat i;

 cpFloat i_inv;


 cpVect p;

 cpVect v;

 cpVect f;



 cpFloat a;

 cpFloat w;

 cpFloat t;



 cpVect rot;




 cpDataPointer data;


 cpFloat v_limit;

 cpFloat w_limit;

 cpVect v_bias_private;
 cpFloat w_bias_private;

 cpSpace *space_private;

 cpShape *shapeList_private;
 cpArbiter *arbiterList_private;
 cpConstraint *constraintList_private;

 cpComponentNode node_private;
};


cpBody* cpBodyAlloc(void);

cpBody* cpBodyInit(cpBody *body, cpFloat m, cpFloat i);

cpBody* cpBodyNew(cpFloat m, cpFloat i);


cpBody* cpBodyInitStatic(cpBody *body);

cpBody* cpBodyNewStatic(void);


void cpBodyDestroy(cpBody *body);

void cpBodyFree(cpBody *body);





 void cpBodySanityCheck(cpBody *body);





void cpBodyActivate(cpBody *body);

void cpBodyActivateStatic(cpBody *body, cpShape *filter);


void cpBodySleep(cpBody *body);

void cpBodySleepWithGroup(cpBody *body, cpBody *group);


cpBool cpBodyIsSleeping(const cpBody *body)
{
 return (body->node_private.root != ((cpBody*)0));
}


cpBool cpBodyIsStatic(const cpBody *body)
{
 return body->node_private.idleTime == (__builtin_inff());
}


cpBool cpBodyIsRogue(const cpBody *body)
{
 return (body->space_private == ((cpSpace*)0));
}
cpFloat cpBodyGetMass(const cpBody *body){return body->m;};

void cpBodySetMass(cpBody *body, cpFloat m);

cpFloat cpBodyGetMoment(const cpBody *body){return body->i;};

void cpBodySetMoment(cpBody *body, cpFloat i);

cpVect cpBodyGetPos(const cpBody *body){return body->p;};

void cpBodySetPos(cpBody *body, cpVect pos);
cpVect cpBodyGetVel(const cpBody *body){return body->v;} void cpBodySetVel(cpBody *body, const cpVect value){ cpBodyActivate(body); cpBodySanityCheck(body); body->v = value; };
cpVect cpBodyGetForce(const cpBody *body){return body->f;} void cpBodySetForce(cpBody *body, const cpVect value){ cpBodyActivate(body); cpBodySanityCheck(body); body->f = value; };
cpFloat cpBodyGetAngle(const cpBody *body){return body->a;};

void cpBodySetAngle(cpBody *body, cpFloat a);
cpFloat cpBodyGetAngVel(const cpBody *body){return body->w;} void cpBodySetAngVel(cpBody *body, const cpFloat value){ cpBodyActivate(body); cpBodySanityCheck(body); body->w = value; };
cpFloat cpBodyGetTorque(const cpBody *body){return body->t;} void cpBodySetTorque(cpBody *body, const cpFloat value){ cpBodyActivate(body); cpBodySanityCheck(body); body->t = value; };
cpVect cpBodyGetRot(const cpBody *body){return body->rot;};
cpFloat cpBodyGetVelLimit(const cpBody *body){return body->v_limit;} void cpBodySetVelLimit(cpBody *body, const cpFloat value){ cpBodyActivate(body); cpBodySanityCheck(body); body->v_limit = value; };
cpFloat cpBodyGetAngVelLimit(const cpBody *body){return body->w_limit;} void cpBodySetAngVelLimit(cpBody *body, const cpFloat value){ cpBodyActivate(body); cpBodySanityCheck(body); body->w_limit = value; };
cpDataPointer cpBodyGetUserData(const cpBody *body){return body->data;} void cpBodySetUserData(cpBody *body, const cpDataPointer value){ cpBodyActivate(body); cpBodySanityCheck(body); body->data = value; };


void cpBodyUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
void cpBodyUpdatePosition(cpBody *body, cpFloat dt);


cpVect cpBodyLocal2World(const cpBody *body, const cpVect v)
{
 return cpvadd(body->p, cpvrotate(v, body->rot));
}


cpVect cpBodyWorld2Local(const cpBody *body, const cpVect v)
{
 return cpvunrotate(cpvsub(v, body->p), body->rot);
}


void cpBodyResetForces(cpBody *body);

void cpBodyApplyForce(cpBody *body, const cpVect f, const cpVect r);

void cpBodyApplyImpulse(cpBody *body, const cpVect j, const cpVect r);


cpVect cpBodyGetVelAtWorldPoint(cpBody *body, cpVect point);

cpVect cpBodyGetVelAtLocalPoint(cpBody *body, cpVect point);



cpFloat cpBodyKineticEnergy(const cpBody *body)
{

 cpFloat vsq = cpvdot(body->v, body->v);
 cpFloat wsq = body->w*body->w;
 return (vsq ? vsq*body->m : 0.0f) + (wsq ? wsq*body->i : 0.0f);
}


typedef void (*cpBodyShapeIteratorFunc)(cpBody *body, cpShape *shape, void *data);

void cpBodyEachShape(cpBody *body, cpBodyShapeIteratorFunc func, void *data);


typedef void (*cpBodyConstraintIteratorFunc)(cpBody *body, cpConstraint *constraint, void *data);

void cpBodyEachConstraint(cpBody *body, cpBodyConstraintIteratorFunc func, void *data);


typedef void (*cpBodyArbiterIteratorFunc)(cpBody *body, cpArbiter *arbiter, void *data);

void cpBodyEachArbiter(cpBody *body, cpBodyArbiterIteratorFunc func, void *data);
typedef struct cpShapeClass cpShapeClass;


typedef struct cpSegmentQueryInfo {

 cpShape *shape;

 cpFloat t;

 cpVect n;
} cpSegmentQueryInfo;


typedef enum cpShapeType{
 CP_CIRCLE_SHAPE,
 CP_SEGMENT_SHAPE,
 CP_POLY_SHAPE,
 CP_NUM_SHAPES
} cpShapeType;

typedef cpBB (*cpShapeCacheDataImpl)(cpShape *shape, cpVect p, cpVect rot);
typedef void (*cpShapeDestroyImpl)(cpShape *shape);
typedef cpBool (*cpShapePointQueryImpl)(cpShape *shape, cpVect p);
typedef void (*cpShapeSegmentQueryImpl)(cpShape *shape, cpVect a, cpVect b, cpSegmentQueryInfo *info);


struct cpShapeClass {
 cpShapeType type;

 cpShapeCacheDataImpl cacheData;
 cpShapeDestroyImpl destroy;
 cpShapePointQueryImpl pointQuery;
 cpShapeSegmentQueryImpl segmentQuery;
};


struct cpShape {
 const cpShapeClass *klass_private;


 cpBody *body;


 cpBB bb;



 cpBool sensor;


 cpFloat e;

 cpFloat u;

 cpVect surface_v;




 cpDataPointer data;


 cpCollisionType collision_type;

 cpGroup group;

 cpLayers layers;

 cpSpace *space_private;

 cpShape *next_private;
 cpShape *prev_private;

 cpHashValue hashid_private;
};


void cpShapeDestroy(cpShape *shape);

void cpShapeFree(cpShape *shape);


cpBB cpShapeCacheBB(cpShape *shape);

cpBB cpShapeUpdate(cpShape *shape, cpVect pos, cpVect rot);


cpBool cpShapePointQuery(cpShape *shape, cpVect p);
cpBody* cpShapeGetBody(const cpShape *shape){return shape->body;};
void cpShapeSetBody(cpShape *shape, cpBody *body);

cpBB cpShapeGetBB(const cpShape *shape){return shape->bb;};
cpBool cpShapeGetSensor(const cpShape *shape){return shape->sensor;} void cpShapeSetSensor(cpShape *shape, cpBool value){ if(1 && shape->body) cpBodyActivate(shape->body); shape->sensor = value; };
cpFloat cpShapeGetElasticity(const cpShape *shape){return shape->e;} void cpShapeSetElasticity(cpShape *shape, cpFloat value){ if(0 && shape->body) cpBodyActivate(shape->body); shape->e = value; };
cpFloat cpShapeGetFriction(const cpShape *shape){return shape->u;} void cpShapeSetFriction(cpShape *shape, cpFloat value){ if(1 && shape->body) cpBodyActivate(shape->body); shape->u = value; };
cpVect cpShapeGetSurfaceVelocity(const cpShape *shape){return shape->surface_v;} void cpShapeSetSurfaceVelocity(cpShape *shape, cpVect value){ if(1 && shape->body) cpBodyActivate(shape->body); shape->surface_v = value; };
cpDataPointer cpShapeGetUserData(const cpShape *shape){return shape->data;} void cpShapeSetUserData(cpShape *shape, cpDataPointer value){ if(0 && shape->body) cpBodyActivate(shape->body); shape->data = value; };
cpCollisionType cpShapeGetCollisionType(const cpShape *shape){return shape->collision_type;} void cpShapeSetCollisionType(cpShape *shape, cpCollisionType value){ if(1 && shape->body) cpBodyActivate(shape->body); shape->collision_type = value; };
cpGroup cpShapeGetGroup(const cpShape *shape){return shape->group;} void cpShapeSetGroup(cpShape *shape, cpGroup value){ if(1 && shape->body) cpBodyActivate(shape->body); shape->group = value; };
cpLayers cpShapeGetLayers(const cpShape *shape){return shape->layers;} void cpShapeSetLayers(cpShape *shape, cpLayers value){ if(1 && shape->body) cpBodyActivate(shape->body); shape->layers = value; };




void cpResetShapeIdCounter(void);


cpBool cpShapeSegmentQuery(cpShape *shape, cpVect a, cpVect b, cpSegmentQueryInfo *info);


cpVect cpSegmentQueryHitPoint(const cpVect start, const cpVect end, const cpSegmentQueryInfo info)
{
 return cpvlerp(start, end, info.t);
}


cpFloat cpSegmentQueryHitDist(const cpVect start, const cpVect end, const cpSegmentQueryInfo info)
{
 return cpvdist(start, end)*info.t;
}







typedef struct cpCircleShape {
 cpShape shape;

 cpVect c, tc;
 cpFloat r;
} cpCircleShape;


cpCircleShape* cpCircleShapeAlloc(void);

cpCircleShape* cpCircleShapeInit(cpCircleShape *circle, cpBody *body, cpFloat radius, cpVect offset);

cpShape* cpCircleShapeNew(cpBody *body, cpFloat radius, cpVect offset);

cpVect cpCircleShapeGetOffset(const cpShape *shape);
cpFloat cpCircleShapeGetRadius(const cpShape *shape);





typedef struct cpSegmentShape {
 cpShape shape;

 cpVect a, b, n;
 cpVect ta, tb, tn;
 cpFloat r;

 cpVect a_tangent, b_tangent;
} cpSegmentShape;


cpSegmentShape* cpSegmentShapeAlloc(void);

cpSegmentShape* cpSegmentShapeInit(cpSegmentShape *seg, cpBody *body, cpVect a, cpVect b, cpFloat radius);

cpShape* cpSegmentShapeNew(cpBody *body, cpVect a, cpVect b, cpFloat radius);

void cpSegmentShapeSetNeighbors(cpShape *shape, cpVect prev, cpVect next);

cpVect cpSegmentShapeGetA(const cpShape *shape);
cpVect cpSegmentShapeGetB(const cpShape *shape);
cpVect cpSegmentShapeGetNormal(const cpShape *shape);
cpFloat cpSegmentShapeGetRadius(const cpShape *shape);
typedef struct cpPolyShapeAxis {
 cpVect n;
 cpFloat d;
} cpPolyShapeAxis;


typedef struct cpPolyShape {
 cpShape shape;

 int numVerts;
 cpVect *verts, *tVerts;
 cpPolyShapeAxis *axes, *tAxes;
} cpPolyShape;


cpPolyShape* cpPolyShapeAlloc(void);


cpPolyShape* cpPolyShapeInit(cpPolyShape *poly, cpBody *body, int numVerts, cpVect *verts, cpVect offset);


cpShape* cpPolyShapeNew(cpBody *body, int numVerts, cpVect *verts, cpVect offset);


cpPolyShape* cpBoxShapeInit(cpPolyShape *poly, cpBody *body, cpFloat width, cpFloat height);

cpPolyShape* cpBoxShapeInit2(cpPolyShape *poly, cpBody *body, cpBB box);

cpShape* cpBoxShapeNew(cpBody *body, cpFloat width, cpFloat height);

cpShape* cpBoxShapeNew2(cpBody *body, cpBB box);


cpBool cpPolyValidate(const cpVect *verts, const int numVerts);


int cpPolyShapeGetNumVerts(cpShape *shape);

cpVect cpPolyShapeGetVert(cpShape *shape, int idx);

typedef cpBool (*cpCollisionBeginFunc)(cpArbiter *arb, cpSpace *space, void *data);


typedef cpBool (*cpCollisionPreSolveFunc)(cpArbiter *arb, cpSpace *space, void *data);

typedef void (*cpCollisionPostSolveFunc)(cpArbiter *arb, cpSpace *space, void *data);

typedef void (*cpCollisionSeparateFunc)(cpArbiter *arb, cpSpace *space, void *data);


struct cpCollisionHandler {
 cpCollisionType a;
 cpCollisionType b;
 cpCollisionBeginFunc begin;
 cpCollisionPreSolveFunc preSolve;
 cpCollisionPostSolveFunc postSolve;
 cpCollisionSeparateFunc separate;
 void *data;
};

typedef struct cpContact cpContact;




typedef enum cpArbiterState {

 cpArbiterStateFirstColl,

 cpArbiterStateNormal,


 cpArbiterStateIgnore,

 cpArbiterStateCached,
} cpArbiterState;


struct cpArbiterThread {

 struct cpArbiter *next, *prev;
};


struct cpArbiter {


 cpFloat e;


 cpFloat u;


 cpVect surface_vr;

 cpShape *a_private;
 cpShape *b_private;
 cpBody *body_a_private;
 cpBody *body_b_private;

 struct cpArbiterThread thread_a_private;
 struct cpArbiterThread thread_b_private;

 int numContacts_private;
 cpContact *contacts_private;

 cpTimestamp stamp_private;
 cpCollisionHandler *handler_private;
 cpBool swappedColl_private;
 cpArbiterState state_private;
};
cpFloat cpArbiterGetElasticity(const cpArbiter *arb){return arb->e;} void cpArbiterSetElasticity(cpArbiter *arb, cpFloat value){arb->e = value;};
cpFloat cpArbiterGetFriction(const cpArbiter *arb){return arb->u;} void cpArbiterSetFriction(cpArbiter *arb, cpFloat value){arb->u = value;};
cpVect cpArbiterGetSurfaceVelocity(const cpArbiter *arb){return arb->surface_vr;} void cpArbiterSetSurfaceVelocity(cpArbiter *arb, cpVect value){arb->surface_vr = value;};



cpVect cpArbiterTotalImpulse(const cpArbiter *arb);


cpVect cpArbiterTotalImpulseWithFriction(const cpArbiter *arb);


cpFloat cpArbiterTotalKE(const cpArbiter *arb);





void cpArbiterIgnore(cpArbiter *arb);




void cpArbiterGetShapes(const cpArbiter *arb, cpShape **a, cpShape **b)
{
 if(arb->swappedColl_private){
  (*a) = arb->b_private, (*b) = arb->a_private;
 } else {
  (*a) = arb->a_private, (*b) = arb->b_private;
 }
}






void cpArbiterGetBodies(const cpArbiter *arb, cpBody **a, cpBody **b)
{
 cpShape *shape_a, *shape_b; cpArbiterGetShapes(arb, &shape_a, &shape_b);;
 (*a) = shape_a->body;
 (*b) = shape_b->body;
}




cpBool cpArbiterIsFirstContact(const cpArbiter *arb)
{
 return arb->state_private == cpArbiterStateFirstColl;
}


int cpArbiterGetCount(const cpArbiter *arb)
{
 return arb->numContacts_private;
}


typedef struct cpContactPointSet {

 int count;


 struct {

  cpVect point;

  cpVect normal;

  cpFloat dist;
 } points[4];
} cpContactPointSet;

cpContactPointSet cpArbiterGetContactPointSet(const cpArbiter *arb);


cpVect cpArbiterGetNormal(const cpArbiter *arb, int i);

cpVect cpArbiterGetPoint(const cpArbiter *arb, int i);

cpFloat cpArbiterGetDepth(const cpArbiter *arb, int i);
typedef struct cpConstraintClass cpConstraintClass;

typedef void (*cpConstraintPreStepImpl)(cpConstraint *constraint, cpFloat dt);
typedef void (*cpConstraintApplyCachedImpulseImpl)(cpConstraint *constraint, cpFloat dt_coef);
typedef void (*cpConstraintApplyImpulseImpl)(cpConstraint *constraint);
typedef cpFloat (*cpConstraintGetImpulseImpl)(cpConstraint *constraint);


struct cpConstraintClass {
 cpConstraintPreStepImpl preStep;
 cpConstraintApplyCachedImpulseImpl applyCachedImpulse;
 cpConstraintApplyImpulseImpl applyImpulse;
 cpConstraintGetImpulseImpl getImpulse;
};


typedef void (*cpConstraintPreSolveFunc)(cpConstraint *constraint, cpSpace *space);

typedef void (*cpConstraintPostSolveFunc)(cpConstraint *constraint, cpSpace *space);



struct cpConstraint {
 const cpConstraintClass *klass_private;


 cpBody *a;

 cpBody *b;

 cpSpace *space_private;

 cpConstraint *next_a_private;
 cpConstraint *next_b_private;



 cpFloat maxForce;



 cpFloat errorBias;


 cpFloat maxBias;



 cpConstraintPreSolveFunc preSolve;



 cpConstraintPostSolveFunc postSolve;




 cpDataPointer data;
};


void cpConstraintDestroy(cpConstraint *constraint);

void cpConstraintFree(cpConstraint *constraint);


void cpConstraintActivateBodies(cpConstraint *constraint)
{
 cpBody *a = constraint->a; if(a) cpBodyActivate(a);
 cpBody *b = constraint->b; if(b) cpBodyActivate(b);
}
cpBody* cpConstraintGetA(const cpConstraint *constraint){return constraint->a;};
cpBody* cpConstraintGetB(const cpConstraint *constraint){return constraint->b;};
cpFloat cpConstraintGetMaxForce(const cpConstraint *constraint){return constraint->maxForce;} void cpConstraintSetMaxForce(cpConstraint *constraint, cpFloat value){ cpConstraintActivateBodies(constraint); constraint->maxForce = value; };
cpFloat cpConstraintGetErrorBias(const cpConstraint *constraint){return constraint->errorBias;} void cpConstraintSetErrorBias(cpConstraint *constraint, cpFloat value){ cpConstraintActivateBodies(constraint); constraint->errorBias = value; };
cpFloat cpConstraintGetMaxBias(const cpConstraint *constraint){return constraint->maxBias;} void cpConstraintSetMaxBias(cpConstraint *constraint, cpFloat value){ cpConstraintActivateBodies(constraint); constraint->maxBias = value; };
cpConstraintPreSolveFunc cpConstraintGetPreSolveFunc(const cpConstraint *constraint){return constraint->preSolve;} void cpConstraintSetPreSolveFunc(cpConstraint *constraint, cpConstraintPreSolveFunc value){ cpConstraintActivateBodies(constraint); constraint->preSolve = value; };
cpConstraintPostSolveFunc cpConstraintGetPostSolveFunc(const cpConstraint *constraint){return constraint->postSolve;} void cpConstraintSetPostSolveFunc(cpConstraint *constraint, cpConstraintPostSolveFunc value){ cpConstraintActivateBodies(constraint); constraint->postSolve = value; };
cpDataPointer cpConstraintGetUserData(const cpConstraint *constraint){return constraint->data;} void cpConstraintSetUserData(cpConstraint *constraint, cpDataPointer value){ cpConstraintActivateBodies(constraint); constraint->data = value; };


cpFloat cpConstraintGetImpulse(cpConstraint *constraint)
{
 return constraint->klass_private->getImpulse(constraint);
}
const cpConstraintClass *cpPinJointGetClass(void);


typedef struct cpPinJoint {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpFloat dist;

 cpVect r1, r2;
 cpVect n;
 cpFloat nMass;

 cpFloat jnAcc, jnMax;
 cpFloat bias;
} cpPinJoint;


cpPinJoint* cpPinJointAlloc(void);

cpPinJoint* cpPinJointInit(cpPinJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2);

cpConstraint* cpPinJointNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2);

cpVect cpPinJointGetAnchr1(const cpConstraint *constraint){ if(!(constraint->klass_private == cpPinJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPinJointGetClass()", "chipmunk/constraints/cpPinJoint.h", 48, 1, 1, "Constraint is not a ""cpPinJoint"); return ((cpPinJoint *)constraint)->anchr1; } void cpPinJointSetAnchr1(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpPinJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPinJointGetClass()", "chipmunk/constraints/cpPinJoint.h", 48, 1, 1, "Constraint is not a ""cpPinJoint"); cpConstraintActivateBodies(constraint); ((cpPinJoint *)constraint)->anchr1 = value; };
cpVect cpPinJointGetAnchr2(const cpConstraint *constraint){ if(!(constraint->klass_private == cpPinJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPinJointGetClass()", "chipmunk/constraints/cpPinJoint.h", 49, 1, 1, "Constraint is not a ""cpPinJoint"); return ((cpPinJoint *)constraint)->anchr2; } void cpPinJointSetAnchr2(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpPinJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPinJointGetClass()", "chipmunk/constraints/cpPinJoint.h", 49, 1, 1, "Constraint is not a ""cpPinJoint"); cpConstraintActivateBodies(constraint); ((cpPinJoint *)constraint)->anchr2 = value; };
cpFloat cpPinJointGetDist(const cpConstraint *constraint){ if(!(constraint->klass_private == cpPinJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPinJointGetClass()", "chipmunk/constraints/cpPinJoint.h", 50, 1, 1, "Constraint is not a ""cpPinJoint"); return ((cpPinJoint *)constraint)->dist; } void cpPinJointSetDist(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpPinJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPinJointGetClass()", "chipmunk/constraints/cpPinJoint.h", 50, 1, 1, "Constraint is not a ""cpPinJoint"); cpConstraintActivateBodies(constraint); ((cpPinJoint *)constraint)->dist = value; };
const cpConstraintClass *cpSlideJointGetClass(void);


typedef struct cpSlideJoint {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpFloat min, max;

 cpVect r1, r2;
 cpVect n;
 cpFloat nMass;

 cpFloat jnAcc, jnMax;
 cpFloat bias;
} cpSlideJoint;


cpSlideJoint* cpSlideJointAlloc(void);

cpSlideJoint* cpSlideJointInit(cpSlideJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat min, cpFloat max);

cpConstraint* cpSlideJointNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat min, cpFloat max);

cpVect cpSlideJointGetAnchr1(const cpConstraint *constraint){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 48, 1, 1, "Constraint is not a ""cpSlideJoint"); return ((cpSlideJoint *)constraint)->anchr1; } void cpSlideJointSetAnchr1(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 48, 1, 1, "Constraint is not a ""cpSlideJoint"); cpConstraintActivateBodies(constraint); ((cpSlideJoint *)constraint)->anchr1 = value; };
cpVect cpSlideJointGetAnchr2(const cpConstraint *constraint){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 49, 1, 1, "Constraint is not a ""cpSlideJoint"); return ((cpSlideJoint *)constraint)->anchr2; } void cpSlideJointSetAnchr2(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 49, 1, 1, "Constraint is not a ""cpSlideJoint"); cpConstraintActivateBodies(constraint); ((cpSlideJoint *)constraint)->anchr2 = value; };
cpFloat cpSlideJointGetMin(const cpConstraint *constraint){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 50, 1, 1, "Constraint is not a ""cpSlideJoint"); return ((cpSlideJoint *)constraint)->min; } void cpSlideJointSetMin(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 50, 1, 1, "Constraint is not a ""cpSlideJoint"); cpConstraintActivateBodies(constraint); ((cpSlideJoint *)constraint)->min = value; };
cpFloat cpSlideJointGetMax(const cpConstraint *constraint){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 51, 1, 1, "Constraint is not a ""cpSlideJoint"); return ((cpSlideJoint *)constraint)->max; } void cpSlideJointSetMax(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpSlideJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSlideJointGetClass()", "chipmunk/constraints/cpSlideJoint.h", 51, 1, 1, "Constraint is not a ""cpSlideJoint"); cpConstraintActivateBodies(constraint); ((cpSlideJoint *)constraint)->max = value; };
const cpConstraintClass *cpPivotJointGetClass(void);


typedef struct cpPivotJoint {
 cpConstraint constraint;
 cpVect anchr1, anchr2;

 cpVect r1, r2;
 cpVect k1, k2;

 cpVect jAcc;
 cpFloat jMaxLen;
 cpVect bias;
} cpPivotJoint;


cpPivotJoint* cpPivotJointAlloc(void);

cpPivotJoint* cpPivotJointInit(cpPivotJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2);

cpConstraint* cpPivotJointNew(cpBody *a, cpBody *b, cpVect pivot);

cpConstraint* cpPivotJointNew2(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2);

cpVect cpPivotJointGetAnchr1(const cpConstraint *constraint){ if(!(constraint->klass_private == cpPivotJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPivotJointGetClass()", "chipmunk/constraints/cpPivotJoint.h", 49, 1, 1, "Constraint is not a ""cpPivotJoint"); return ((cpPivotJoint *)constraint)->anchr1; } void cpPivotJointSetAnchr1(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpPivotJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPivotJointGetClass()", "chipmunk/constraints/cpPivotJoint.h", 49, 1, 1, "Constraint is not a ""cpPivotJoint"); cpConstraintActivateBodies(constraint); ((cpPivotJoint *)constraint)->anchr1 = value; };
cpVect cpPivotJointGetAnchr2(const cpConstraint *constraint){ if(!(constraint->klass_private == cpPivotJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPivotJointGetClass()", "chipmunk/constraints/cpPivotJoint.h", 50, 1, 1, "Constraint is not a ""cpPivotJoint"); return ((cpPivotJoint *)constraint)->anchr2; } void cpPivotJointSetAnchr2(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpPivotJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpPivotJointGetClass()", "chipmunk/constraints/cpPivotJoint.h", 50, 1, 1, "Constraint is not a ""cpPivotJoint"); cpConstraintActivateBodies(constraint); ((cpPivotJoint *)constraint)->anchr2 = value; };
const cpConstraintClass *cpGrooveJointGetClass(void);


typedef struct cpGrooveJoint {
 cpConstraint constraint;
 cpVect grv_n, grv_a, grv_b;
 cpVect anchr2;

 cpVect grv_tn;
 cpFloat clamp;
 cpVect r1, r2;
 cpVect k1, k2;

 cpVect jAcc;
 cpFloat jMaxLen;
 cpVect bias;
} cpGrooveJoint;


cpGrooveJoint* cpGrooveJointAlloc(void);

cpGrooveJoint* cpGrooveJointInit(cpGrooveJoint *joint, cpBody *a, cpBody *b, cpVect groove_a, cpVect groove_b, cpVect anchr2);

cpConstraint* cpGrooveJointNew(cpBody *a, cpBody *b, cpVect groove_a, cpVect groove_b, cpVect anchr2);

cpVect cpGrooveJointGetGrooveA(const cpConstraint *constraint){ if(!(constraint->klass_private == cpGrooveJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGrooveJointGetClass()", "chipmunk/constraints/cpGrooveJoint.h", 50, 1, 1, "Constraint is not a ""cpGrooveJoint"); return ((cpGrooveJoint *)constraint)->grv_a; };

void cpGrooveJointSetGrooveA(cpConstraint *constraint, cpVect value);
cpVect cpGrooveJointGetGrooveB(const cpConstraint *constraint){ if(!(constraint->klass_private == cpGrooveJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGrooveJointGetClass()", "chipmunk/constraints/cpGrooveJoint.h", 53, 1, 1, "Constraint is not a ""cpGrooveJoint"); return ((cpGrooveJoint *)constraint)->grv_b; };

void cpGrooveJointSetGrooveB(cpConstraint *constraint, cpVect value);
cpVect cpGrooveJointGetAnchr2(const cpConstraint *constraint){ if(!(constraint->klass_private == cpGrooveJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGrooveJointGetClass()", "chipmunk/constraints/cpGrooveJoint.h", 56, 1, 1, "Constraint is not a ""cpGrooveJoint"); return ((cpGrooveJoint *)constraint)->anchr2; } void cpGrooveJointSetAnchr2(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpGrooveJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGrooveJointGetClass()", "chipmunk/constraints/cpGrooveJoint.h", 56, 1, 1, "Constraint is not a ""cpGrooveJoint"); cpConstraintActivateBodies(constraint); ((cpGrooveJoint *)constraint)->anchr2 = value; };
typedef struct cpDampedSpring cpDampedSpring;

typedef cpFloat (*cpDampedSpringForceFunc)(cpConstraint *spring, cpFloat dist);

const cpConstraintClass *cpDampedSpringGetClass(void);


struct cpDampedSpring {
 cpConstraint constraint;
 cpVect anchr1, anchr2;
 cpFloat restLength;
 cpFloat stiffness;
 cpFloat damping;
 cpDampedSpringForceFunc springForceFunc;

 cpFloat target_vrn;
 cpFloat v_coef;

 cpVect r1, r2;
 cpFloat nMass;
 cpVect n;
};


cpDampedSpring* cpDampedSpringAlloc(void);

cpDampedSpring* cpDampedSpringInit(cpDampedSpring *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat restLength, cpFloat stiffness, cpFloat damping);

cpConstraint* cpDampedSpringNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2, cpFloat restLength, cpFloat stiffness, cpFloat damping);

cpVect cpDampedSpringGetAnchr1(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 55, 1, 1, "Constraint is not a ""cpDampedSpring"); return ((cpDampedSpring *)constraint)->anchr1; } void cpDampedSpringSetAnchr1(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 55, 1, 1, "Constraint is not a ""cpDampedSpring"); cpConstraintActivateBodies(constraint); ((cpDampedSpring *)constraint)->anchr1 = value; };
cpVect cpDampedSpringGetAnchr2(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 56, 1, 1, "Constraint is not a ""cpDampedSpring"); return ((cpDampedSpring *)constraint)->anchr2; } void cpDampedSpringSetAnchr2(cpConstraint *constraint, cpVect value){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 56, 1, 1, "Constraint is not a ""cpDampedSpring"); cpConstraintActivateBodies(constraint); ((cpDampedSpring *)constraint)->anchr2 = value; };
cpFloat cpDampedSpringGetRestLength(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 57, 1, 1, "Constraint is not a ""cpDampedSpring"); return ((cpDampedSpring *)constraint)->restLength; } void cpDampedSpringSetRestLength(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 57, 1, 1, "Constraint is not a ""cpDampedSpring"); cpConstraintActivateBodies(constraint); ((cpDampedSpring *)constraint)->restLength = value; };
cpFloat cpDampedSpringGetStiffness(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 58, 1, 1, "Constraint is not a ""cpDampedSpring"); return ((cpDampedSpring *)constraint)->stiffness; } void cpDampedSpringSetStiffness(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 58, 1, 1, "Constraint is not a ""cpDampedSpring"); cpConstraintActivateBodies(constraint); ((cpDampedSpring *)constraint)->stiffness = value; };
cpFloat cpDampedSpringGetDamping(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 59, 1, 1, "Constraint is not a ""cpDampedSpring"); return ((cpDampedSpring *)constraint)->damping; } void cpDampedSpringSetDamping(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 59, 1, 1, "Constraint is not a ""cpDampedSpring"); cpConstraintActivateBodies(constraint); ((cpDampedSpring *)constraint)->damping = value; };
cpDampedSpringForceFunc cpDampedSpringGetSpringForceFunc(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 60, 1, 1, "Constraint is not a ""cpDampedSpring"); return ((cpDampedSpring *)constraint)->springForceFunc; } void cpDampedSpringSetSpringForceFunc(cpConstraint *constraint, cpDampedSpringForceFunc value){ if(!(constraint->klass_private == cpDampedSpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedSpringGetClass()", "chipmunk/constraints/cpDampedSpring.h", 60, 1, 1, "Constraint is not a ""cpDampedSpring"); cpConstraintActivateBodies(constraint); ((cpDampedSpring *)constraint)->springForceFunc = value; };
typedef cpFloat (*cpDampedRotarySpringTorqueFunc)(struct cpConstraint *spring, cpFloat relativeAngle);

const cpConstraintClass *cpDampedRotarySpringGetClass(void);


typedef struct cpDampedRotarySpring {
 cpConstraint constraint;
 cpFloat restAngle;
 cpFloat stiffness;
 cpFloat damping;
 cpDampedRotarySpringTorqueFunc springTorqueFunc;

 cpFloat target_wrn;
 cpFloat w_coef;

 cpFloat iSum;
} cpDampedRotarySpring;


cpDampedRotarySpring* cpDampedRotarySpringAlloc(void);

cpDampedRotarySpring* cpDampedRotarySpringInit(cpDampedRotarySpring *joint, cpBody *a, cpBody *b, cpFloat restAngle, cpFloat stiffness, cpFloat damping);

cpConstraint* cpDampedRotarySpringNew(cpBody *a, cpBody *b, cpFloat restAngle, cpFloat stiffness, cpFloat damping);

cpFloat cpDampedRotarySpringGetRestAngle(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 50, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); return ((cpDampedRotarySpring *)constraint)->restAngle; } void cpDampedRotarySpringSetRestAngle(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 50, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); cpConstraintActivateBodies(constraint); ((cpDampedRotarySpring *)constraint)->restAngle = value; };
cpFloat cpDampedRotarySpringGetStiffness(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 51, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); return ((cpDampedRotarySpring *)constraint)->stiffness; } void cpDampedRotarySpringSetStiffness(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 51, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); cpConstraintActivateBodies(constraint); ((cpDampedRotarySpring *)constraint)->stiffness = value; };
cpFloat cpDampedRotarySpringGetDamping(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 52, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); return ((cpDampedRotarySpring *)constraint)->damping; } void cpDampedRotarySpringSetDamping(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 52, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); cpConstraintActivateBodies(constraint); ((cpDampedRotarySpring *)constraint)->damping = value; };
cpDampedRotarySpringTorqueFunc cpDampedRotarySpringGetSpringTorqueFunc(const cpConstraint *constraint){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 53, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); return ((cpDampedRotarySpring *)constraint)->springTorqueFunc; } void cpDampedRotarySpringSetSpringTorqueFunc(cpConstraint *constraint, cpDampedRotarySpringTorqueFunc value){ if(!(constraint->klass_private == cpDampedRotarySpringGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpDampedRotarySpringGetClass()", "chipmunk/constraints/cpDampedRotarySpring.h", 53, 1, 1, "Constraint is not a ""cpDampedRotarySpring"); cpConstraintActivateBodies(constraint); ((cpDampedRotarySpring *)constraint)->springTorqueFunc = value; };
const cpConstraintClass *cpRotaryLimitJointGetClass(void);


typedef struct cpRotaryLimitJoint {
 cpConstraint constraint;
 cpFloat min, max;

 cpFloat iSum;

 cpFloat bias;
 cpFloat jAcc, jMax;
} cpRotaryLimitJoint;


cpRotaryLimitJoint* cpRotaryLimitJointAlloc(void);

cpRotaryLimitJoint* cpRotaryLimitJointInit(cpRotaryLimitJoint *joint, cpBody *a, cpBody *b, cpFloat min, cpFloat max);

cpConstraint* cpRotaryLimitJointNew(cpBody *a, cpBody *b, cpFloat min, cpFloat max);

cpFloat cpRotaryLimitJointGetMin(const cpConstraint *constraint){ if(!(constraint->klass_private == cpRotaryLimitJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRotaryLimitJointGetClass()", "chipmunk/constraints/cpRotaryLimitJoint.h", 45, 1, 1, "Constraint is not a ""cpRotaryLimitJoint"); return ((cpRotaryLimitJoint *)constraint)->min; } void cpRotaryLimitJointSetMin(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpRotaryLimitJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRotaryLimitJointGetClass()", "chipmunk/constraints/cpRotaryLimitJoint.h", 45, 1, 1, "Constraint is not a ""cpRotaryLimitJoint"); cpConstraintActivateBodies(constraint); ((cpRotaryLimitJoint *)constraint)->min = value; };
cpFloat cpRotaryLimitJointGetMax(const cpConstraint *constraint){ if(!(constraint->klass_private == cpRotaryLimitJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRotaryLimitJointGetClass()", "chipmunk/constraints/cpRotaryLimitJoint.h", 46, 1, 1, "Constraint is not a ""cpRotaryLimitJoint"); return ((cpRotaryLimitJoint *)constraint)->max; } void cpRotaryLimitJointSetMax(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpRotaryLimitJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRotaryLimitJointGetClass()", "chipmunk/constraints/cpRotaryLimitJoint.h", 46, 1, 1, "Constraint is not a ""cpRotaryLimitJoint"); cpConstraintActivateBodies(constraint); ((cpRotaryLimitJoint *)constraint)->max = value; };
const cpConstraintClass *cpRatchetJointGetClass(void);


typedef struct cpRatchetJoint {
 cpConstraint constraint;
 cpFloat angle, phase, ratchet;

 cpFloat iSum;

 cpFloat bias;
 cpFloat jAcc, jMax;
} cpRatchetJoint;


cpRatchetJoint* cpRatchetJointAlloc(void);

cpRatchetJoint* cpRatchetJointInit(cpRatchetJoint *joint, cpBody *a, cpBody *b, cpFloat phase, cpFloat ratchet);

cpConstraint* cpRatchetJointNew(cpBody *a, cpBody *b, cpFloat phase, cpFloat ratchet);

cpFloat cpRatchetJointGetAngle(const cpConstraint *constraint){ if(!(constraint->klass_private == cpRatchetJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRatchetJointGetClass()", "chipmunk/constraints/cpRatchetJoint.h", 45, 1, 1, "Constraint is not a ""cpRatchetJoint"); return ((cpRatchetJoint *)constraint)->angle; } void cpRatchetJointSetAngle(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpRatchetJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRatchetJointGetClass()", "chipmunk/constraints/cpRatchetJoint.h", 45, 1, 1, "Constraint is not a ""cpRatchetJoint"); cpConstraintActivateBodies(constraint); ((cpRatchetJoint *)constraint)->angle = value; };
cpFloat cpRatchetJointGetPhase(const cpConstraint *constraint){ if(!(constraint->klass_private == cpRatchetJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRatchetJointGetClass()", "chipmunk/constraints/cpRatchetJoint.h", 46, 1, 1, "Constraint is not a ""cpRatchetJoint"); return ((cpRatchetJoint *)constraint)->phase; } void cpRatchetJointSetPhase(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpRatchetJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRatchetJointGetClass()", "chipmunk/constraints/cpRatchetJoint.h", 46, 1, 1, "Constraint is not a ""cpRatchetJoint"); cpConstraintActivateBodies(constraint); ((cpRatchetJoint *)constraint)->phase = value; };
cpFloat cpRatchetJointGetRatchet(const cpConstraint *constraint){ if(!(constraint->klass_private == cpRatchetJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRatchetJointGetClass()", "chipmunk/constraints/cpRatchetJoint.h", 47, 1, 1, "Constraint is not a ""cpRatchetJoint"); return ((cpRatchetJoint *)constraint)->ratchet; } void cpRatchetJointSetRatchet(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpRatchetJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpRatchetJointGetClass()", "chipmunk/constraints/cpRatchetJoint.h", 47, 1, 1, "Constraint is not a ""cpRatchetJoint"); cpConstraintActivateBodies(constraint); ((cpRatchetJoint *)constraint)->ratchet = value; };
const cpConstraintClass *cpGearJointGetClass(void);


typedef struct cpGearJoint {
 cpConstraint constraint;
 cpFloat phase, ratio;
 cpFloat ratio_inv;

 cpFloat iSum;

 cpFloat bias;
 cpFloat jAcc, jMax;
} cpGearJoint;


cpGearJoint* cpGearJointAlloc(void);

cpGearJoint* cpGearJointInit(cpGearJoint *joint, cpBody *a, cpBody *b, cpFloat phase, cpFloat ratio);

cpConstraint* cpGearJointNew(cpBody *a, cpBody *b, cpFloat phase, cpFloat ratio);

cpFloat cpGearJointGetPhase(const cpConstraint *constraint){ if(!(constraint->klass_private == cpGearJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGearJointGetClass()", "chipmunk/constraints/cpGearJoint.h", 46, 1, 1, "Constraint is not a ""cpGearJoint"); return ((cpGearJoint *)constraint)->phase; } void cpGearJointSetPhase(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpGearJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGearJointGetClass()", "chipmunk/constraints/cpGearJoint.h", 46, 1, 1, "Constraint is not a ""cpGearJoint"); cpConstraintActivateBodies(constraint); ((cpGearJoint *)constraint)->phase = value; };
cpFloat cpGearJointGetRatio(const cpConstraint *constraint){ if(!(constraint->klass_private == cpGearJointGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpGearJointGetClass()", "chipmunk/constraints/cpGearJoint.h", 47, 1, 1, "Constraint is not a ""cpGearJoint"); return ((cpGearJoint *)constraint)->ratio; };

void cpGearJointSetRatio(cpConstraint *constraint, cpFloat value);
const cpConstraintClass *cpSimpleMotorGetClass(void);


typedef struct cpSimpleMotor {
 cpConstraint constraint;
 cpFloat rate;

 cpFloat iSum;

 cpFloat jAcc, jMax;
} cpSimpleMotor;


cpSimpleMotor* cpSimpleMotorAlloc(void);

cpSimpleMotor* cpSimpleMotorInit(cpSimpleMotor *joint, cpBody *a, cpBody *b, cpFloat rate);

cpConstraint* cpSimpleMotorNew(cpBody *a, cpBody *b, cpFloat rate);

cpFloat cpSimpleMotorGetRate(const cpConstraint *constraint){ if(!(constraint->klass_private == cpSimpleMotorGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSimpleMotorGetClass()", "chipmunk/constraints/cpSimpleMotor.h", 44, 1, 1, "Constraint is not a ""cpSimpleMotor"); return ((cpSimpleMotor *)constraint)->rate; } void cpSimpleMotorSetRate(cpConstraint *constraint, cpFloat value){ if(!(constraint->klass_private == cpSimpleMotorGetClass())) cpMessage("constraint->CP_PRIVATE(klass) == cpSimpleMotorGetClass()", "chipmunk/constraints/cpSimpleMotor.h", 44, 1, 1, "Constraint is not a ""cpSimpleMotor"); cpConstraintActivateBodies(constraint); ((cpSimpleMotor *)constraint)->rate = value; };

typedef struct cpContactBufferHeader cpContactBufferHeader;
typedef void (*cpSpaceArbiterApplyImpulseFunc)(cpArbiter *arb);


struct cpSpace {

 int iterations;


 cpVect gravity;





 cpFloat damping;



 cpFloat idleSpeedThreshold;




 cpFloat sleepTimeThreshold;





 cpFloat collisionSlop;




 cpFloat collisionBias;



 cpTimestamp collisionPersistence;



 cpBool enableContactGraph;




 cpDataPointer data;




 cpBody *staticBody;

 cpTimestamp stamp_private;
 cpFloat curr_dt_private;

 cpArray *bodies_private;
 cpArray *rousedBodies_private;
 cpArray *sleepingComponents_private;

 cpSpatialIndex *staticShapes_private;
 cpSpatialIndex *activeShapes_private;

 cpArray *arbiters_private;
 cpContactBufferHeader *contactBuffersHead_private;
 cpHashSet *cachedArbiters_private;
 cpArray *pooledArbiters_private;
 cpArray *constraints_private;

 cpArray *allocatedBuffers_private;
 int locked_private;

 cpHashSet *collisionHandlers_private;
 cpCollisionHandler defaultHandler_private;
 cpHashSet *postStepCallbacks_private;

 cpSpaceArbiterApplyImpulseFunc arbiterApplyImpulse_private;

 cpBody _staticBody_private;
};


cpSpace* cpSpaceAlloc(void);

cpSpace* cpSpaceInit(cpSpace *space);

cpSpace* cpSpaceNew(void);


void cpSpaceDestroy(cpSpace *space);

void cpSpaceFree(cpSpace *space);
int cpSpaceGetIterations(const cpSpace *space){return space->iterations;} void cpSpaceSetIterations(cpSpace *space, int value){space->iterations = value;};
cpVect cpSpaceGetGravity(const cpSpace *space){return space->gravity;} void cpSpaceSetGravity(cpSpace *space, cpVect value){space->gravity = value;};
cpFloat cpSpaceGetDamping(const cpSpace *space){return space->damping;} void cpSpaceSetDamping(cpSpace *space, cpFloat value){space->damping = value;};
cpFloat cpSpaceGetIdleSpeedThreshold(const cpSpace *space){return space->idleSpeedThreshold;} void cpSpaceSetIdleSpeedThreshold(cpSpace *space, cpFloat value){space->idleSpeedThreshold = value;};
cpFloat cpSpaceGetSleepTimeThreshold(const cpSpace *space){return space->sleepTimeThreshold;} void cpSpaceSetSleepTimeThreshold(cpSpace *space, cpFloat value){space->sleepTimeThreshold = value;};
cpFloat cpSpaceGetCollisionSlop(const cpSpace *space){return space->collisionSlop;} void cpSpaceSetCollisionSlop(cpSpace *space, cpFloat value){space->collisionSlop = value;};
cpFloat cpSpaceGetCollisionBias(const cpSpace *space){return space->collisionBias;} void cpSpaceSetCollisionBias(cpSpace *space, cpFloat value){space->collisionBias = value;};
cpTimestamp cpSpaceGetCollisionPersistence(const cpSpace *space){return space->collisionPersistence;} void cpSpaceSetCollisionPersistence(cpSpace *space, cpTimestamp value){space->collisionPersistence = value;};
cpBool cpSpaceGetEnableContactGraph(const cpSpace *space){return space->enableContactGraph;} void cpSpaceSetEnableContactGraph(cpSpace *space, cpBool value){space->enableContactGraph = value;};
cpDataPointer cpSpaceGetUserData(const cpSpace *space){return space->data;} void cpSpaceSetUserData(cpSpace *space, cpDataPointer value){space->data = value;};
cpBody* cpSpaceGetStaticBody(const cpSpace *space){return space->staticBody;};
cpFloat cpSpaceGetCurrentTimeStep(const cpSpace *space){return space->curr_dt_private;};


cpBool
cpSpaceIsLocked(cpSpace *space)
{
 return space->locked_private;
}





void cpSpaceSetDefaultCollisionHandler(
 cpSpace *space,
 cpCollisionBeginFunc begin,
 cpCollisionPreSolveFunc preSolve,
 cpCollisionPostSolveFunc postSolve,
 cpCollisionSeparateFunc separate,
 void *data
);



void cpSpaceAddCollisionHandler(
 cpSpace *space,
 cpCollisionType a, cpCollisionType b,
 cpCollisionBeginFunc begin,
 cpCollisionPreSolveFunc preSolve,
 cpCollisionPostSolveFunc postSolve,
 cpCollisionSeparateFunc separate,
 void *data
);


void cpSpaceRemoveCollisionHandler(cpSpace *space, cpCollisionType a, cpCollisionType b);



cpShape* cpSpaceAddShape(cpSpace *space, cpShape *shape);

cpShape* cpSpaceAddStaticShape(cpSpace *space, cpShape *shape);

cpBody* cpSpaceAddBody(cpSpace *space, cpBody *body);

cpConstraint* cpSpaceAddConstraint(cpSpace *space, cpConstraint *constraint);


void cpSpaceRemoveShape(cpSpace *space, cpShape *shape);

void cpSpaceRemoveStaticShape(cpSpace *space, cpShape *shape);

void cpSpaceRemoveBody(cpSpace *space, cpBody *body);

void cpSpaceRemoveConstraint(cpSpace *space, cpConstraint *constraint);


cpBool cpSpaceContainsShape(cpSpace *space, cpShape *shape);

cpBool cpSpaceContainsBody(cpSpace *space, cpBody *body);

cpBool cpSpaceContainsConstraint(cpSpace *space, cpConstraint *constraint);


typedef void (*cpPostStepFunc)(cpSpace *space, void *obj, void *data);


void cpSpaceAddPostStepCallback(cpSpace *space, cpPostStepFunc func, void *obj, void *data);


typedef void (*cpSpacePointQueryFunc)(cpShape *shape, void *data);

void cpSpacePointQuery(cpSpace *space, cpVect point, cpLayers layers, cpGroup group, cpSpacePointQueryFunc func, void *data);

cpShape *cpSpacePointQueryFirst(cpSpace *space, cpVect point, cpLayers layers, cpGroup group);


typedef void (*cpSpaceSegmentQueryFunc)(cpShape *shape, cpFloat t, cpVect n, void *data);

void cpSpaceSegmentQuery(cpSpace *space, cpVect start, cpVect end, cpLayers layers, cpGroup group, cpSpaceSegmentQueryFunc func, void *data);

cpShape *cpSpaceSegmentQueryFirst(cpSpace *space, cpVect start, cpVect end, cpLayers layers, cpGroup group, cpSegmentQueryInfo *out);


typedef void (*cpSpaceBBQueryFunc)(cpShape *shape, void *data);


void cpSpaceBBQuery(cpSpace *space, cpBB bb, cpLayers layers, cpGroup group, cpSpaceBBQueryFunc func, void *data);


typedef void (*cpSpaceShapeQueryFunc)(cpShape *shape, cpContactPointSet *points, void *data);

cpBool cpSpaceShapeQuery(cpSpace *space, cpShape *shape, cpSpaceShapeQueryFunc func, void *data);


void cpSpaceActivateShapesTouchingShape(cpSpace *space, cpShape *shape);



typedef void (*cpSpaceBodyIteratorFunc)(cpBody *body, void *data);

void cpSpaceEachBody(cpSpace *space, cpSpaceBodyIteratorFunc func, void *data);


typedef void (*cpSpaceShapeIteratorFunc)(cpShape *shape, void *data);

void cpSpaceEachShape(cpSpace *space, cpSpaceShapeIteratorFunc func, void *data);


typedef void (*cpSpaceConstraintIteratorFunc)(cpConstraint *constraint, void *data);

void cpSpaceEachConstraint(cpSpace *space, cpSpaceConstraintIteratorFunc func, void *data);


void cpSpaceReindexStatic(cpSpace *space);

void cpSpaceReindexShape(cpSpace *space, cpShape *shape);

void cpSpaceReindexShapesForBody(cpSpace *space, cpBody *body);


void cpSpaceUseSpatialHash(cpSpace *space, cpFloat dim, int count);


void cpSpaceStep(cpSpace *space, cpFloat dt);







extern const char *cpVersionString;


void cpInitChipmunk(void);



cpFloat cpMomentForCircle(cpFloat m, cpFloat r1, cpFloat r2, cpVect offset);



cpFloat cpAreaForCircle(cpFloat r1, cpFloat r2);



cpFloat cpMomentForSegment(cpFloat m, cpVect a, cpVect b);


cpFloat cpAreaForSegment(cpVect a, cpVect b, cpFloat r);


cpFloat cpMomentForPoly(cpFloat m, int numVerts, const cpVect *verts, cpVect offset);



cpFloat cpAreaForPoly(const int numVerts, const cpVect *verts);


cpVect cpCentroidForPoly(const int numVerts, const cpVect *verts);


void cpRecenterPoly(const int numVerts, cpVect *verts);


cpFloat cpMomentForBox(cpFloat m, cpFloat width, cpFloat height);


cpFloat cpMomentForBox2(cpFloat m, cpBB box);
