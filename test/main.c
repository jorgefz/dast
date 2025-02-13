
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_arch/test_arch.h"
#include "test_mem/test_mem.h"
#include "test_array/test_array.h"


int main(int argc, const char* argv[]){

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
        TEST_GROUP_MEM,
        TEST_GROUP_ARRAY
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}