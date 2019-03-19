#include <Amber/util/common.h>
#include <Amber/util/vector.h>
#include <Amber/graphics/objloader.h>

int AB_load_obj(FILE *infile, struct AB_mesh_info *out)
{
    if (infile == NULL || out == NULL) {
        AB_QUICK_LOG("[Objloader] AB_load_obj() passed NULL parameter");
        return 1;
    }

    /* Setup output structure */
    AB_VEC_INIT(&out->vertices);
    AB_VEC_INIT(&out->tex_coords);
    AB_VEC_INIT(&out->normals);
    out->name = NULL;

    /* Temporary structures */
    AB_vec(vec3) vertices_tmp     = AB_VEC_INITIALIZER;
    AB_vec(vec2) tex_coords_tmp   = AB_VEC_INITIALIZER;
    AB_vec(vec3) normals_tmp      = AB_VEC_INITIALIZER;
    AB_vec(int) vertex_indices    = AB_VEC_INITIALIZER;
    AB_vec(int) tex_coord_indices = AB_VEC_INITIALIZER;
    AB_vec(int) normal_indices    = AB_VEC_INITIALIZER;

    /* Expect a lot of members */
    AB_VEC_RESIZE(&vertices_tmp,      256, AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&tex_coords_tmp,    256, AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&normals_tmp,       256, AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&vertex_indices,    128, AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&tex_coord_indices, 128, AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&normal_indices,    128, AB_VEC_ONFAIL(goto cleanup_err));

    /* Parse the input line-by-line */
    char *line = NULL;
    size_t n = 0;
    while (AB_GETLINE(&line, &n, infile) > 0) {
        float a, b, c;
        int ai, bi, ci, di, ei, fi, gi, hi, ii;
        if (sscanf(line, "v %f %f %f", &a, &b, &c) == 3) {
            /* Vertex location */
            vec3 *vert;
            AB_VEC_PUSHP(&vertices_tmp, &vert,
                    AB_VEC_ONFAIL(goto cleanup_err));
            (*vert)[0] = a, (*vert)[1] = b, (*vert)[2] = c;
        } else if (sscanf(line, "vt %f %f", &a, &b) == 2) {
            /* Vertex texture-coord */
            vec2 *coord;
            AB_VEC_PUSHP(&tex_coords_tmp, &coord, 
                    AB_VEC_ONFAIL(goto cleanup_err));
            (*coord)[0] = a, (*coord)[1] = b;
        } else if (sscanf(line, "vn %f %f %f", &a, &b, &c) == 3) {
            /* Vertex normal */
            vec3 *norm;
            AB_VEC_PUSHP(&normals_tmp, &norm, AB_VEC_ONFAIL(goto cleanup_err));
            (*norm)[0] = a, (*norm)[1] = b, (*norm)[2] = c;
        } else if (sscanf(line, "f %f %f %f", &a, &b, &c) == 3) {
            /* Triangle with location */
            AB_VEC_PUSH(&vertex_indices, a, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, b, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, c, AB_VEC_ONFAIL(goto cleanup_err));
        } else if (sscanf(line, "f %d//%d %d//%d %d//%d",
                    &ai, &bi, &ci, &di, &ei, &fi) == 6) {
            /* Triangle with location and normal */
            AB_VEC_PUSH(&vertex_indices, ai, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, ci, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, ei, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&normal_indices, bi, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&normal_indices, di, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&normal_indices, fi, AB_VEC_ONFAIL(goto cleanup_err));
        } else if (sscanf(line, "f %d/%d %d/%d %d/%d",
                    &ai, &bi, &ci, &di, &ei, &fi) == 6) {
            /* Triangle with location and texture-coord */
            AB_VEC_PUSH(&vertex_indices, ai, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, ci, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, ei, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&tex_coord_indices, bi, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&tex_coord_indices, di, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&tex_coord_indices, fi, AB_VEC_ONFAIL(goto cleanup_err));
        } else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &ai, &bi, &ci, &di, &ei, &fi, &gi, &hi, &ii) == 9) {
            /* Triangle with location, texture-coord and normal */
            AB_VEC_PUSH(&vertex_indices, ai, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, di, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&vertex_indices, gi, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&tex_coord_indices, bi, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&tex_coord_indices, ei, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&tex_coord_indices, hi, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&normal_indices, ci, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&normal_indices, fi, AB_VEC_ONFAIL(goto cleanup_err));
            AB_VEC_PUSH(&normal_indices, ii, AB_VEC_ONFAIL(goto cleanup_err));
        } else if (strncmp(line, "o ", 2) == 0) {
            if (out->name)
                free(out->name);
            out->name = AB_STRDUP(line + 2);
        }
    }
    free(line);

    /* Resize the output vectors to the correct size */
    AB_VEC_RESIZE(&out->vertices,   AB_VEC_NUM(&vertex_indices),    AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&out->tex_coords, AB_VEC_NUM(&tex_coord_indices), AB_VEC_ONFAIL(goto cleanup_err));
    AB_VEC_RESIZE(&out->normals,    AB_VEC_NUM(&normal_indices),    AB_VEC_ONFAIL(goto cleanup_err));

    /* Convert from .obj format to a usable one */
    int i;
    for (i = 0; i < AB_VEC_NUM(&vertex_indices); i++) {
        int vert_idx = AB_VEC_AT(&vertex_indices, i);
        if (vert_idx < 0) {
            vert_idx = AB_VEC_NUM(&tex_coord_indices) + vert_idx;
        }
        if (vert_idx >= 0 && vert_idx <= AB_VEC_NUM(&vertices_tmp)) {
            vec3 *vertex = &AB_VEC_AT(&vertices_tmp, vert_idx - 1);
            vec3 *vertex0;
            AB_VEC_PUSHP(&out->vertices, &vertex0, AB_VEC_ONFAIL(goto cleanup_err));
            memcpy(vertex0, vertex, sizeof(vec3));
        } else {
            AB_QUICK_LOG(
                    "[Objloader] Vertex index %u is beyond the number of vertices %u",
                    vert_idx, AB_VEC_NUM(&vertices_tmp));
            goto cleanup_err;
        }
    }
    for (i = 0; i < AB_VEC_NUM(&tex_coord_indices); i++) {
        int tex_idx = AB_VEC_AT(&tex_coord_indices, i);
        if (tex_idx < 0) {
            tex_idx = AB_VEC_NUM(&tex_coord_indices) + tex_idx;
        }
        if (tex_idx >= 0 && tex_idx <= AB_VEC_NUM(&tex_coords_tmp)) {
            vec2 *tex_coord = &AB_VEC_AT(&tex_coords_tmp, tex_idx - 1);
            vec2 *tex_coord0;
            AB_VEC_PUSHP(&out->tex_coords, &tex_coord0, AB_VEC_ONFAIL(goto cleanup_err));
            memcpy(tex_coord0, tex_coord, sizeof(vec2));
        } else {
            AB_QUICK_LOG(
                    "[Objloader] Vertex index %u is beyond the number of vertices %u",
                    tex_idx, AB_VEC_NUM(&tex_coords_tmp));
            goto cleanup_err;
        }
    }
    for (i = 0; i < AB_VEC_NUM(&normal_indices); i++) {
        int norm_idx = AB_VEC_AT(&normal_indices, i);
        if (norm_idx < 0) {
            norm_idx = AB_VEC_NUM(&tex_coord_indices) + norm_idx;
        }
        if (norm_idx >= 0 && norm_idx <= AB_VEC_NUM(&normals_tmp)) {
            vec3 *norm = &AB_VEC_AT(&normals_tmp, norm_idx - 1);
            vec3 *norm0;
            AB_VEC_PUSHP(&out->normals, &norm0, AB_VEC_ONFAIL(goto cleanup_err));
            memcpy(norm0, norm, sizeof(vec3));
        } else {
            AB_QUICK_LOG(
                    "[Objloader] Normal index %u is beyond the number of normals %u",
                    norm_idx, AB_VEC_NUM(&normals_tmp));
            goto cleanup_err;
        }
    }

    /* Cleanup tmp structures */
    AB_VEC_DESTROY(&vertices_tmp);
    AB_VEC_DESTROY(&tex_coords_tmp);
    AB_VEC_DESTROY(&normals_tmp);
    AB_VEC_DESTROY(&normal_indices);
    AB_VEC_DESTROY(&vertex_indices);
    AB_VEC_DESTROY(&tex_coord_indices);
    return 0;

cleanup_err:
    /* Cleanup tmp structures */
    AB_VEC_DESTROY(&vertices_tmp);
    AB_VEC_DESTROY(&tex_coords_tmp);
    AB_VEC_DESTROY(&normals_tmp);
    AB_VEC_DESTROY(&normal_indices);
    AB_VEC_DESTROY(&vertex_indices);
    AB_VEC_DESTROY(&tex_coord_indices);
    /* Cleanup input structures */
    AB_VEC_DESTROY(&out->vertices);
    AB_VEC_DESTROY(&out->tex_coords);
    AB_VEC_DESTROY(&out->normals);
    free(out->name);
    return 1;

}
