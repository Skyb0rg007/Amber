#include <Amber/util/vector.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

int main(void)
{
    AB_vec(int) my_vec = AB_VEC_INITIALIZER;
    int i;

    for (i = 0; i < 20; i++) {
        int ret = AB_vec_push(&my_vec, i);
        assert(ret == 0);
    }

    for (i = 0; i < 20; i++) {
        printf("%d -> %d\n", i, AB_vec_at(&my_vec, i));
    }

    for (i = 0; i < 20; i++) {
        AB_vec_insert(&my_vec, 20 + (size_t)i, 2);
        int *a = AB_vec_pushp(&my_vec);
        *a = i * 2;
    }

    printf("vec: { .size = %u, .capacity = %u }\n",
            AB_vec_size(&my_vec), AB_vec_capacity(&my_vec));

    while (AB_vec_size(&my_vec) > 0) {
        int val;
        AB_vec_pop(&my_vec, &val);
        printf("pop -> %d\n", val);
    }

    AB_vec(unsigned) another_vec = AB_VEC_INITIALIZER;
    AB_vec_copy(&another_vec, &my_vec);

    AB_vec_destroy(&my_vec);
    AB_vec_destroy(&another_vec);
}
