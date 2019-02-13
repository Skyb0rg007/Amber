#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include <Amber/util/common.h>
#include <Amber/graphics/objloader.h>

int main(int argc, char *argv[])
{
    FILE *infile = argc > 1 ? fopen(argv[1], "rt") : stdin;
    if (!infile)
        return 1;
    struct AB_mesh_info info;
    int ret = AB_load_obj(infile, &info);
    if (ret != 0) {
        fclose(infile);
        return 1;
    }

    printf("info: { .name = %s, #vertices = %u, #tex_coords = %u, #normals = %u }\n",
            info.name,
            AB_vec_size(&info.vertices), AB_vec_size(&info.tex_coords), AB_vec_size(&info.normals));
    /* for (unsigned i = 0; i < AB_vec_size(&info.vertices); i++) */
        /* glm_vec3_print(AB_vec_at(&info.vertices, i), stdout); */
    /* for (unsigned i = 0; i < AB_vec_size(&info.tex_coords); i++) */
        /* glm_vec2_print(AB_vec_at(&info.tex_coords, i), stdout); */
    /* for (unsigned i = 0; i < AB_vec_size(&info.normals); i++) */
        /* glm_vec3_print(AB_vec_at(&info.normals, i), stdout); */

    AB_vec_destroy(&info.vertices);
    AB_vec_destroy(&info.normals);
    AB_vec_destroy(&info.tex_coords);
    free(info.name);

    if (infile != stdin)
        fclose(infile);
    return 0;
}
