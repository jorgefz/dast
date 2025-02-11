/* 
 * +--------------+
 * |    Macros    |
 * +--------------+
 * 
 * +-----------------+----------------------------------------+
 * | Macro           | Description                            |
 * +-----------------+----------------------------------------+
 * | DAST_NO_STDLIB  | Disables all standard library includes |
 * | DAST_ALLOC      | Custom global memory alloc             |
 * | DAST_REALLOC    | Custom global memory realloc           |
 * | DAST_FREE       | Custom global memory free              |
 * | DAST_HASH_64BIT | Enables 64-bit hashes                  |
 * | 
 * 
 */


#ifndef DAST_DEFS_H
#define DAST_DEFS_H


/* Check architecture */

/* Windows */
#if defined(_WIN32) || defined(_WIN64)
    #if defined(_WIN64)
        #define DAST_64BIT
    #else
        #define  DAST_32BIT
    #endif
/* Linux - GCC */
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(__x86_64__) || defined(__ppc64__)
        #define DAST_64BIT
    #else
        #define DAST_32BIT
    #endif
#endif


#ifdef DAST_NO_STDLIB

    /* Fixed types */
    #if defined(DAST_32BIT)
    
        typedef          char  dast_i8;
        typedef unsigned char  dast_u8;
        typedef          short dast_i16;
        typedef unsigned short dast_u16;
        typedef          int   dast_i32;
        typedef unsigned int   dast_u32;
        typedef unsigned long  dast_sz;

        #if defined(_MSC_VER)
            typedef          __int64 dast_i64;
            typedef unsigned __int64 dast_u64;
        #else
            typedef          long long dast_i64;
            typedef unsigned long long dast_u64;
        #endif

    #elif defined(DAST_64BIT)
    
        typedef          char  dast_i8;
        typedef unsigned char  dast_u8;
        typedef          short dast_i16;
        typedef unsigned short dast_u16;
        typedef          int   dast_i32;
        typedef unsigned int   dast_u32;
        typedef          long  dast_i64;
        typedef unsigned long  dast_u64;
        typedef unsigned long  dast_sz;
    
    #endif

    /* Constants */
    #define NULL ((void*)0)

#else

    #include <string.h> /* memcmp, memcpy, memmove */
    #include <stdint.h> /* sized types (e.g. uint32_t) */
    #include <stddef.h> /* size_t, NULL */
    #include <stdlib.h> /* malloc, realloc, free */

    /* Fixed types */
    typedef int8_t   dast_i8;
    typedef uint8_t  dast_u8;
    typedef int16_t  dast_i16;
    typedef uint16_t dast_u16;
    typedef int32_t  dast_i32;
    typedef uint32_t dast_u32;
    typedef int64_t  dast_i64;
    typedef uint64_t dast_u64;
    typedef size_t   dast_sz;

#endif

typedef dast_u32 dast_bool; /**< Boolean type */
#define dast_true  (dast_bool)1 /**< Boolean true */
#define dast_false (dast_bool)0 /**< Boolean false */

/* Memory allocation */
typedef void* (*dast_alloc_t)  (dast_sz size);                 /**< Typedef for memory allocation function */ 
typedef void* (*dast_realloc_t)(void* block, dast_sz newsize); /**< Typedef for memory reallocation function */
typedef void  (*dast_free_t)   (void* block);                  /**< Typedef for memory deallocation function */

/** Memory management interface */
typedef struct dast_allocator {
    dast_alloc_t   alloc;   /**< Allocation function   */
    dast_realloc_t realloc; /**< Reallocation function */
    dast_free_t    free;    /**< Deallocation function */
} dast_allocator_t;

/* Default allocator */
#ifdef DAST_NO_STDLIB
    #define DAST_DEFAULT_ALLOCATOR (dast_allocator_t){0}
#else
    #define DAST_DEFAULT_ALLOCATOR (dast_allocator_t){malloc, realloc, free}
#endif

/* Hashing */
#ifdef DAST_HASH_64BIT
    typedef dast_u64 dast_hash_t; /**< 64-bit hash type */
#else
    typedef dast_u32 dast_hash_t; /**< 32-bit hash type */
#endif


#endif /* DAST_DEFS_H */

