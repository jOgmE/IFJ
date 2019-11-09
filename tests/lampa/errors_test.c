#include "../../lib/errors.h"

void print_err(error *err, char result_char)
{
    printf("[%c] Current error: \n", result_char);
    printf("[%c] err->type: %d (%s) \n", result_char, err->type, error_type_names[err->type]);
    printf("[%c] err->severity: %d (%s) \n", result_char, err->severity, error_sev_names[err->severity]);
    printf("[%c] err->message: %s \n\n", result_char, err->message);
}

void test_err_list()
{
    // ****************************************************************************//
    // ****************** init test 01 ********************************************//
    // ****************************************************************************//

    printf("[-] init_error(err, NULL_ERROR, ERROR, \"NULL_ERROR initialization test\")\n");

    error *err = (error *)malloc(sizeof(error));

    error_type init_result = init_error(err, NULL_ERROR, ERROR, "NULL_ERROR initialization test");

    if (init_result != SUCCESS)
    {
        printf("[N] init_error(err, NULL_ERROR, ERROR, \"NULL_ERROR initialization test\") not SUCCESS\n");
        print_err(err, 'N');
    }

    printf("[Y] init_error(err, NULL_ERROR, ERROR, \"NULL_ERROR initialization test\") test passed\n\n");
    print_err(err, 'Y');

    // ****************************************************************************//
    // ****************** init list test 01 ***************************************//
    // ****************************************************************************//

    printf("[-] init_error_list(err_list)\n");

    error_list *err_list = (error_list *)malloc(sizeof(error_list));

    init_result = init_error_list(err_list);

    if (init_result != SUCCESS)
    {
        printf("[N] init_error_list(err_list) not SUCCESS\n");
    }

    printf("[Y] init_error_list(err_list) test passed\n\n");

    // ****************************************************************************//
    // ****************** add test 01 *********************************************//
    // ****************************************************************************//

    printf("[-] add_error(err_list, err)\n");

    init_result = add_error(err_list, err);

    if (init_result != SUCCESS)
    {
        printf("[N] add_error(err_list, err) not SUCCESS\n");
    }

    printf("[Y] add_error(err_list, err) test passed\n\n");

    printf("[Y] Printing error list:\n");
    print_errors(err_list);
    printf("\n");

    // ****************************************************************************//
    // ****************** init test 02 ********************************************//
    // ****************************************************************************//

    printf("[-] init_and_add_error(err_list, MEMORY_ERROR, WARNING, \"MEMORY_ERROR initialization test\")\n");

    init_result = init_and_add_error(err_list, MEMORY_ERROR, WARNING, "MEMORY_ERROR initialization test");

    if (init_result != SUCCESS)
    {
        printf("[N] init_and_add_error(err_list, MEMORY_ERROR, WARNING, \"MEMORY_ERROR initialization test\") not SUCCESS\n");
    }

    printf("[Y] init_and_add_error(err_list, MEMORY_ERROR, WARNING, \"MEMORY_ERROR initialization test\") test passed\n\n");

    printf("[Y] Printing error list:\n");
    print_errors(err_list);
    printf("\n");

    // ****************************************************************************//
    // ****************** free ****************************************************//
    // ****************************************************************************//

    free(err_list->first_error->next_err);
    free(err_list->first_error);
    free(err_list);
}

int main(int argc, char const *argv[])
{
    test_err_list();
    return 0;
}
