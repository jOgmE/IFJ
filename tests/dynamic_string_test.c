#include "../lib/dynamic_string.c"
#include <string.h>

bool test_dynamic_string()
{
    printf("[-] init_cstring(\"a\")\n");
    cstring *first_cstr = init_cstring("a");

    if (first_cstr == NULL)
    {
        printf("[N] ERROR init_cstring(\"a\") returned NULL\n");
        exit(1);
    }

    printf("[Y] alloc_cstring(\"a\") test passed\n\n");

    printf("[-] resize_cstring(first_cstr, 8)\n");
    resize_cstring(first_cstr, 8);

    if (first_cstr->allocated_size != 8)
    {
        printf("[N] ERROR resize_cstring(first_cstr, 8) not resized to 8\n");
        exit(1);
    }

    printf("[Y] resize_cstring(first_cstr, 8) test passed\n\n");

    printf("[-] append_char(first_cstr, 'b')\n");
    append_char(first_cstr, 'b');

    if (strcmp("ab", first_cstr->str) != 0)
    {
        printf("[N] ERROR append_char(first_cstr, 'b') returned %s (not equal to \"ab\")\n", first_cstr->str);
        exit(1);
    }

    printf("[Y] append_char(first_cstr, 'b') test passed\n\n");

    printf("[-] append_char(first_cstr, 'c')\n");

    resize_cstring(first_cstr, 2);

    append_char(first_cstr, 'c');

    if (strcmp("abc", first_cstr->str) != 0)
    {
        printf("[N] ERROR append_char(cstring*, cstring*) returned %s (not equal to \"abc\")\n", first_cstr->str);
        exit(1);
    }

    printf("[Y] append_char(first_cstr, 'c') test passed (resize)\n\n");

    printf("[-] compare_string(first_cstr, \"abc\")\n");
    if (!compare_string(first_cstr, "abc"))
    {
        printf("[N] ERROR compare_string(first_cstr, \"abc\") returned %s (not equal to \"abc\")\n", first_cstr->str);
        exit(1);
    }

    printf("[Y] compare_string(first_cstr, \"abc\") test 01 passed (\"abc\")\n\n");

    cstring *second_cstr = init_cstring("ab");

    printf("[-] compare_cstring(second_cstr, \"ab\")\n");
    if (compare_cstring(first_cstr, second_cstr))
    {
        printf("[N] ERROR compare_cstring(second_cstr, \"ab\") cstrings are equal (\"%s\")\n", second_cstr->str);
        exit(1);
    }

    printf("[Y] compare_cstring(second_cstr, \"ab\") test passed (not equal)\n\n");

    printf("[-] append_cstring(first_cstr, second_cstr)\n");
    append_cstring(first_cstr, second_cstr);

    if (!compare_string(first_cstr, "abcab"))
    {
        printf("[N] ERROR append_cstring(first_cstr, second_cstr) cstrings not equal to \"abcab\"\n");
        exit(1);
    }

    printf("[Y] append_cstring(first_cstr, second_cstr) test passed\n\n");

    printf("[-] free_cstring(first_cstr)\n");
    free_cstring(first_cstr);
    printf("[Y] free_cstring(first_cstr) test passed\n\n");

    printf("[-] free_cstring(second_cstr)\n");
    free_cstring(second_cstr);
    printf("[Y] free_cstring(second_cstr) test passed\n");
}