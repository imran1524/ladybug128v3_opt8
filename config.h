#ifndef CONFIG_H_
#define CONFIG_H_

//INLINE THE LADYBUG MODE*/
/*
Purpose: This directive determines whether Ascon's main operational mode functions should be inlined.
Default Value: 1 (enabled)
Effect: When set to 1, the compiler is suggested to inline the Ascon mode functions. Inlining is a common optimization that attempts to reduce function call overhead by substituting the function call with its actual code, potentially making the code faster at the cost of increased binary size.
*/
#ifndef LADYBUG_INLINE_MODE
#define LADYBUG_INLINE_MODE 0
#endif

//INLINE ALL PERMUTATIONS
/*
Purpose: Controls whether the permutation functions within Ascon are inlined.
Default Value: 0 (disabled)
Effect: This is set to 0 by default, meaning that permutation functions are not inlined. This choice could be due to permutations possibly being large or complex functions where inlining could adversely affect performance or binary size.
*/
#ifndef LADYBUG_INLINE_PERM
#define LADYBUG_INLINE_PERM 0
#endif

//UNROLL PERMUTATION LOOPS
/*
Purpose: Determines whether loops within the permutation functions are unrolled.
Default Value: 1 (enabled)
Effect: Loop unrolling is an optimization that reduces the loop overhead at the expense of larger code size. It can significantly speed up tightly looped computations by eliminating the need for loop control instructions (e.g., increment and condition check). When set to 1, it suggests that loops within the permutations should be unrolled to potentially enhance performance.
*/
#ifndef LADYBUG_UNROLL_LOOPS
#define LADYBUG_UNROLL_LOOPS 1
#endif

//INLINE BITINTERLEAVING
/*
Purpose: Specifies whether bit interleaving functions are inlined.
Default Value: 1 (enabled)
Effect: Setting this to 1 advises the compiler to inline bit interleaving functions. Bit interleaving might be a candidate for inlining if these functions are called frequently and are not too large, which would make the overhead of calling them repeatedly undesirable.
*/
#ifndef LADYBUG_EXTERM_BI
#define LADYBUG_EXTERM_BI 1
#endif

//EXPERN BITINTERLEAVING
/*
Purpose: Controls the linkage of bit interleaving functions.
Default Value: 1 (enabled)
Effect: When set to 1, this suggests that bit interleaving functions should be treated as external, which is somewhat unusual in combination with inlining. Typically, you would not declare a function both inline and external as inlining suggests internal linkage. This could potentially be a misconfiguration or might require specific handling or conditional compilation directives elsewhere in the code.
*/
#ifndef LADYBUG_EXPERN_BI
#define LADYBUG_EXPERN_BI 1
#endif

#endif // CONFIG_H_