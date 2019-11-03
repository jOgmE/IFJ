#include "../lib/errors.h"
#include <stdio.h>
#include <stdlib.h>

void print_ptr(void *ptr, char state)
{
    printf("[%c] Pointer: %p\n\n", state, ptr);
}

void test_ptr_errors()
{
    error_t err;

    // ***************************************************************************//
    // ****************** NULL ptr test ******************************************//
    // ***************************************************************************//

    printf("[-] check_pointer(NULL)\n");

    err = check_pointer(NULL);

    if (err != INVALID_POINTER)
    {
        printf("[N] ERROR check_pointer(NULL) returned %d\n", err);
        exit(1);
    }

    printf("[Y] check_pointer(NULL) test passed\n\n");

    // ***************************************************************************//
    // ****************** string ptr test ****************************************//
    // ***************************************************************************//

    printf("[-] check_pointer(\"abc\")\n");

    err = check_pointer("abc");

    if (err != SUCCESS)
    {
        printf("[N] ERROR check_pointer(\"abc\") returned %d\n", err);
        print_ptr("abc", 'N');
        exit(1);
    }

    printf("[Y] check_pointer(\"abc\") test passed\n");
    print_ptr("abc", 'Y');

    // **************************************************************************//
    // ****************** valid ptr test ****************************************//
    // **************************************************************************//

    printf("[-] check_pointer(ptr)\n");

    void *ptr = (void *)malloc(1);
    err = check_pointer(ptr);

    if (err != SUCCESS)
    {
        printf("[N] ERROR check_pointer(ptr) returned %d\n", err);
        print_ptr(ptr, 'N');
        exit(1);
    }

    printf("[Y] check_pointer(ptr) test passed\n");
    print_ptr(ptr, 'Y');

    free(ptr);
}

int main(int argc, char const *argv[])
{
    test_ptr_errors();
    return 0;
}
