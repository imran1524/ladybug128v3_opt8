#ifndef FORCEINLINE_H_
#define FORCEINLINE_H_

/* Define forceinline macro for different compilers */

/* Microsoft Compiler */
#ifdef _MSC_VER
#define forceinline __forceinline

/* GNU Compiler */
#elif defined(__GNUC__)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define forceinline inline __attribute__((__always_inline__))
#else
#define forceinline static inline
#endif

/* Clang Compiler */
#elif defined(__clang__)
#if __has_attribute(__always_inline__)
#define forceinline inline __attribute__((__always_inline__))
#else
#define forceinline inline
#endif

/* Other Compilers */
#else
#define forceinline inline
#endif

#endif /* FORCEINLINE_H_ */
