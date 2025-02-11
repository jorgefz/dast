
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

/* TEST_ARCH */

#define TEST_GROUP_ARCH \
    cmocka_unit_test(test_arch), \
    cmocka_unit_test(test_i8),   \
    cmocka_unit_test(test_u8),   \
    cmocka_unit_test(test_i16),  \
    cmocka_unit_test(test_u16),  \
    cmocka_unit_test(test_i32),  \
    cmocka_unit_test(test_u32),  \
    cmocka_unit_test(test_i64),  \
    cmocka_unit_test(test_u64),  \
    cmocka_unit_test(test_bool), \
    cmocka_unit_test(test_sz)

void test_arch(void**);
void test_i8(void**);
void test_u8(void**);
void test_i16(void**);
void test_u16(void**);
void test_i32(void**);
void test_u32(void**);
void test_i64(void**);
void test_u64(void**);
void test_bool(void**);
void test_sz(void**);


/* TEST_MEM */

#define TEST_GROUP_MEM \
    cmocka_unit_test(test_memeq), \
    cmocka_unit_test(test_memset), \
    cmocka_unit_test(test_memcpy), \
    cmocka_unit_test(test_memmove)

void test_memeq(void**);
void test_memset(void**);
void test_memcpy(void**);
void test_memmove(void**);



int main(){

    #ifdef DAST_64BIT
    printf("64-bit mode\n");
    #elif defined(DAST_32BIT)
    printf("32-bit mode\n");
    #endif

    #ifdef DAST_NO_STD
    printf("STD Lib disabled\n");
    #else
    printf("STD Lib enabled\n");
    #endif

    static const struct CMUnitTest tests[] = {
        TEST_GROUP_ARCH,
        TEST_GROUP_MEM
    };
    return cmocka_run_group_tests(tests, NULL, NULL);


}