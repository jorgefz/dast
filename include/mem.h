#ifndef DAST_MEM_H
#define DAST_MEM_H

#include "defs.h"

/** Compares `size` bytes between two memory locations `lhs` and `rhs`.
 * @param lhs First memory location at which to compare data
 * @param rhs Second memory location at which to compare data
 * @param size Number of bytes to compare 
 * @returns `dast_true` if they are equal, and `dast_false` otherwise.
*/
dast_bool dast_memeq(const void* lhs, const void* rhs, dast_sz size);

/** Copies `size` bytes from `src` to `dest`.
 * @param dest Destination
 * @param src Source location
 * @param size Number of bytes to copy
 * @returns `dest` on success, and NULL if `dest` or `src` are NULL.
 * @note It performs a copy forward, and so it is not suitable for overlapping memory segments.
*/
void* dast_memcpy(void* dest, const void* src, dast_sz size);

/** Copies `size` bytes from `src` to `dest`, handling overlapping segments.
 * @param dest Destination
 * @param src Source location
 * @param size Number of bytes to copy
 * @returns `dest` on success, and NULL if `dest` or `src` are NULL.
 * @note When dest <= src, its behaviour is identical to memcpy.
*/
void* dast_memmove(void* dest, const void* src, dast_sz size);

#endif /* DAST_MEM_H */