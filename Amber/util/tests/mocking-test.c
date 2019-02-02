#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void null_test_success(void **state)
{
    (void)state;
}

static void math_test_success(void **state)
{
    (void)state;

    assert_null(NULL);
    char *a = "a";
    char *b = a;
    assert_ptr_equal(a, b);

    assert_int_equal(4, 4);
}

static void example_test_fail(void **state)
{
    (void)state;

    assert_string_equal("This test", "fails!");
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(math_test_success),
        cmocka_unit_test(example_test_fail),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
