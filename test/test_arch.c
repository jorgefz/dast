
#include "defs.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>


void test_i8  (void** state) { assert_int_equal(sizeof(dast_i8),   1); }
void test_u8  (void** state) { assert_int_equal(sizeof(dast_u8),   1); }
void test_i16 (void** state) { assert_int_equal(sizeof(dast_i16),  2); }
void test_u16 (void** state) { assert_int_equal(sizeof(dast_u16),  2); }
void test_i32 (void** state) { assert_int_equal(sizeof(dast_i32),  4); }
void test_u32 (void** state) { assert_int_equal(sizeof(dast_u32),  4); }
void test_i64 (void** state) { assert_int_equal(sizeof(dast_i64),  8); }
void test_u64 (void** state) { assert_int_equal(sizeof(dast_u64),  8); }
void test_bool(void** state) { assert_int_equal(sizeof(dast_bool), 4); }

void test_sz  (void** state) {
#if defined(DAST_32BIT)
    assert_int_equal(sizeof(dast_sz),   4);
#elif defined(DAST_64BIT)
    assert_int_equal(sizeof(dast_sz),   8);
#endif
}

void test_arch(void** state) {
    #if !defined(DAST_32BIT) && !defined(DAST_64BIT)
    assert_true(0);
    #endif
}
