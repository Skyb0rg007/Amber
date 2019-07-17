#include <Amber/util/common.h>

static inline
void *my_failing_realloc(void *ptr, size_t newsize) {
    static int counter = 10;
    counter--;
    if (counter <= 0) {
        AB_QUICK_LOG("Failing realloc!");
        return NULL;
    } else {
        return realloc(ptr, newsize);
    }
}

#define realloc my_failing_realloc
#include <Amber/util/vector.h>
#undef realloc

#include <Amber/cmocka/AB_cmocka.h>

static int num_trials = 20;

static void on_fail_test(void **data)
{
    int i;
    AB_vec(int) my_vec = AB_VEC_INITIALIZER;

    (void)data;

    for (i = 0; i < num_trials; i++) {
        AB_VEC_PUSH(&my_vec, i,
                AB_VEC_ONFAIL(AB_QUICK_LOG("Error!"); goto cleanup));
    }

    printf("Num: %u\n", AB_VEC_NUM(&my_vec));

cleanup:
    AB_VEC_DESTROY(&my_vec);
}

int main(int argc, char *argv[])
{
    const struct CMUnitTest vector_tests[] = {
        cmocka_unit_test(on_fail_test)
    };

    if (argc > 1)
        num_trials = atoi(argv[1]);

    return cmocka_run_group_tests(vector_tests, NULL, NULL);
}
