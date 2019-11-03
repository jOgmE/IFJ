#include <stdio.h>
#include "../lib/dynamic_string.h"
#include <string.h>

void print_state(cstring *cstr, char result_char)
{
    printf("[%c] Current state: \n", result_char);
    printf("[%c] cstr->str: %s \n", result_char, cstr->str);
    printf("[%c] cstr->allocated_size: %zu \n", result_char, cstr->allocated_size);
    printf("[%c] cstr->length: %zu \n\n", result_char, cstr->length);
}

bool test_dynamic_string()
{
    // ****************************************************************************//
    // ****************** init test 01 ********************************************//
    // ****************************************************************************//

    printf("[-] init_cstring(\"a\")\n");
    cstring *first_cstr = init_cstring("a");

    if (first_cstr == NULL)
    {
        printf("[N] ERROR init_cstring(\"a\") returned NULL\n");
        exit(1);
    }

    printf("[Y] init_cstring(\"a\") test passed\n\n");
    print_state(first_cstr, 'Y');

    // ****************************************************************************//
    // ****************** init test 02 ********************************************//
    // ****************************************************************************//

    printf("[-] init_cstring_size(\"1\")\n");
    cstring *third_cstr = init_cstring_size(1);

    if (third_cstr == NULL)
    {
        printf("[N] ERROR init_cstring_size(1) returned NULL\n");
        exit(1);
    }

    printf("[Y] init_cstring_size(1) test passed\n\n");
    print_state(third_cstr, 'Y');

    // ****************************************************************************//
    // ****************** resize test 01 ******************************************//
    // ****************************************************************************//

    printf("[-] resize_cstring(first_cstr, 8)\n");
    resize_cstring(first_cstr, 8);

    if (first_cstr->allocated_size != 8)
    {
        printf("[N] ERROR resize_cstring(first_cstr, 8) not resized to 8\n");
        print_state(first_cstr, 'N');
        exit(1);
    }

    printf("[Y] resize_cstring(first_cstr, 8) test passed\n\n");
    print_state(first_cstr, 'Y');

    // ****************************************************************************//
    // ****************** append test 01 ******************************************//
    // ****************************************************************************//

    printf("[-] append_char(first_cstr, 'b')\n");
    append_char(first_cstr, 'b');

    if (strcmp("ab", first_cstr->str) != 0)
    {
        printf("[N] ERROR append_char(first_cstr, 'b') returned %s (not equal to \"ab\")\n", first_cstr->str);
        print_state(first_cstr, 'N');
        exit(1);
    }

    printf("[Y] append_char(first_cstr, 'b') test passed\n\n");
    print_state(first_cstr, 'Y');

    // ****************************************************************************//
    // ****************** append test 02 ******************************************//
    // ****************************************************************************//

    printf("[-] append_char(first_cstr, 'c')\n");

    resize_cstring(first_cstr, 2);

    append_char(first_cstr, 'c');

    if (strcmp("abc", first_cstr->str) != 0)
    {
        printf("[N] ERROR append_char(cstring*, cstring*) returned %s (not equal to \"abc\")\n", first_cstr->str);
        print_state(first_cstr, 'N');
        exit(1);
    }

    printf("[Y] append_char(first_cstr, 'c') test passed (resize)\n\n");
    print_state(first_cstr, 'Y');

    // ****************************************************************************//
    // ****************** compare test 01 *****************************************//
    // ****************************************************************************//

    printf("[-] compare_string(first_cstr, \"abc\")\n");
    if (!compare_string(first_cstr, "abc"))
    {
        printf("[N] ERROR compare_string(first_cstr, \"abc\") returned %s (not equal to \"abc\")\n", first_cstr->str);
        print_state(first_cstr, 'N');
        exit(1);
    }

    printf("[Y] compare_string(first_cstr, \"abc\") test 01 passed (\"abc\")\n\n");
    print_state(first_cstr, 'Y');

    // ****************************************************************************//
    // ****************** compare test 02 *****************************************//
    // ****************************************************************************//

    cstring *second_cstr = init_cstring("ab");

    printf("[-] compare_cstring(second_cstr, \"ab\")\n");
    if (compare_cstring(first_cstr, second_cstr))
    {
        printf("[N] ERROR compare_cstring(second_cstr, \"ab\") cstrings are equal (\"%s\")\n", second_cstr->str);

        printf("[Y] first_cstr\n");
        print_state(first_cstr, 'N');

        printf("[Y] second_cstr\n");
        print_state(second_cstr, 'N');

        exit(1);
    }

    printf("[Y] compare_cstring(second_cstr, \"ab\") test passed (not equal)\n\n");

    printf("[Y] first_cstr\n");
    print_state(first_cstr, 'Y');

    printf("[Y] second_cstr\n");
    print_state(second_cstr, 'Y');

    // ****************************************************************************//
    // ****************** append test 03 ******************************************//
    // ****************************************************************************//

    printf("[-] append_cstring(first_cstr, second_cstr)\n");
    append_cstring(first_cstr, second_cstr);

    if (!compare_string(first_cstr, "abcab"))
    {
        printf("[N] ERROR append_cstring(first_cstr, second_cstr) cstrings not equal to \"abcab\"\n");
        print_state(first_cstr, 'N');
        exit(1);
    }

    printf("[Y] append_cstring(first_cstr, second_cstr) test passed\n\n");
    print_state(first_cstr, 'Y');

    // ****************************************************************************//
    // ****************** append test 04 ******************************************//
    // ****************************************************************************//

    printf("[-] append_string(third_cstr, \"cd\")\n");
    append_string(third_cstr, "cd");

    if (!compare_string(third_cstr, "cd"))
    {
        printf("[N] ERROR append_string(third_cstr, \"cd\") string not equal to \"cd\" (%s)\n", second_cstr->str);
        print_state(third_cstr, 'N');
        exit(1);
    }

    printf("[Y] append_string(third_cstr, \"cd\") test passed (resize)\n\n");
    print_state(third_cstr, 'Y');

    // ****************************************************************************//
    // ****************** resize test 02 ******************************************//
    // ****************************************************************************//

    printf("[-] resize_cstring(third_cstr, 1)\n");
    resize_cstring(third_cstr, 1);

    if (third_cstr->allocated_size != 1 || third_cstr->length != 1 || !compare_string(third_cstr, "c"))
    {
        printf("[N] ERROR resize_cstring(third_cstr, 1) string not equal to \"c\" (%s)\n", second_cstr->str);
        print_state(third_cstr, 'N');
        exit(1);
    }

    printf("[Y] resize_cstring(third_cstr, 1) test passed (cut string)\n\n");
    print_state(third_cstr, 'Y');

    // ****************************************************************************//
    // ****************** free test 01 ********************************************//
    // ****************************************************************************//

    printf("[-] free_cstring(first_cstr)\n");
    free_cstring(first_cstr);
    printf("[Y] free_cstring(first_cstr) test passed\n\n");

    // ****************************************************************************//
    // ****************** free test 03 ********************************************//
    // ****************************************************************************//

    printf("[-] free_cstring(second_cstr)\n");
    free_cstring(second_cstr);
    printf("[Y] free_cstring(second_cstr) test passed\n");

    // ****************************************************************************//
    // ****************** free test 02 ********************************************//
    // ****************************************************************************//

    printf("[-] free_cstring(third_cstr)\n");
    free_cstring(third_cstr);
    printf("[Y] free_cstring(third_cstr) test passed\n");
}

int main(int argc, char const *argv[])
{
    test_dynamic_string();
    return 0;
}