#include "str.h"


string_t string_from_chars_custom(const char* chars, dast_sz len, dast_allocator_t alloc){
    string_t s;
    if(!alloc.alloc || !alloc.free){
        return (string_t){0};
    }

    s.alloc = alloc;
    s.str = alloc.alloc(len + 1);
    if(!s.str){
        s.len = 0;
        return s;
    }

    if(chars){
        dast_memcpy(s.str, chars, len);
        s.str[len] = (char)0;
    } else {
        dast_memset(s.str, 0, len + 1);
    }

    s.len = len;
    return s;
}

string_t string_from_chars(const char* chars, dast_sz len){
    return string_from_chars_custom(chars, len, DAST_DEFAULT_ALLOCATOR);
}

string_t string_from_len_custom(dast_sz len, dast_allocator_t alloc){
    return string_from_chars_custom(NULL, len, alloc);
}

string_t string_from_len(dast_sz len){
    return string_from_len_custom(len, DAST_DEFAULT_ALLOCATOR);
}

string_t string_from_fmt_custom(dast_allocator_t alloc, const char fmt[], ...){
#ifdef DAST_NO_STDLIB
    (void)fmt;
    (void)alloc;
    return (string_t){0};
#else
    int len;
    string_t s;
    
    /** Two va_list objects needed:
     *  (1) for calculating the maximum number of bytes written to the buffer
     *  (2) for writing to the buffer
     * Cannot reuse only one since it is consumed after use
     */
    va_list arg1, arg2;
    va_start(arg1, fmt);
    va_copy(arg2, arg1);

    /* Fetch expected length without writing to a buffer */
    len = vsnprintf(NULL, 0, fmt, arg1);
    va_end(arg1);

    if(len < 0) return (string_t){0};

    s = string_from_len_custom((dast_sz)len, alloc);
    if(!s.str) return s;

    /* Takes length including null-terminator, but returns length without it */
    vsnprintf(s.str, s.len + 1, fmt, arg2);
    va_end(arg2);
    return s;
#endif
}

string_t string_from_fmt(const char fmt[], ...){
#ifdef DAST_NO_STDLIB
    return (string_t){0};
#else
    /* Must repeat code in string_from_fmt_custom because in C89
     * there is no __VA_ARGS__, so we can't pass the variadic arguments
     * to string_from_fmt_custom
     */
    int len;
    string_t s;
    
    /** Two va_list objects needed:
     *  (1) for calculating the maximum number of bytes written to the buffer
     *  (2) for writing to the buffer
     * Cannot reuse only one since it is consumed after use
     */
    va_list arg1, arg2;
    va_start(arg1, fmt);
    va_copy(arg2, arg1);

    /* Fetch expected length without writing to a buffer */
    len = vsnprintf(NULL, 0, fmt, arg1);
    va_end(arg1);

    if(len < 0) return (string_t){0};

    s = string_from_len((dast_sz)len);
    if(!s.str) return s;

    /* Takes length including null-terminator, but returns length without it */
    vsnprintf(s.str, s.len + 1, fmt, arg2);
    va_end(arg2);
    return s;
#endif
}

void string_free(string_t* str){
    if (!str) return;
    str->len = 0;
    if(str->str) str->alloc.free(str->str);
    str->str = NULL;
}
