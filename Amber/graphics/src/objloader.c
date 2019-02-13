#include <Amber/util/common.h>
#include <Amber/util/vector.h>
#include <Amber/graphics/objloader.h>

int AB_load_obj(FILE *infile, struct AB_mesh_info *out)
{
    if (infile == NULL || out == NULL) {
        AB_LOG_ERROR("[Objloader] AB_load_obj() passed NULL parameter");
        return 1;
    }

    /* Setup output structure */
    AB_vec_init(&out->vertices);
    AB_vec_init(&out->tex_coords);
    AB_vec_init(&out->normals);
    out->name = NULL;

    /* Temporary structures */
    AB_vec(vec3) vertices_tmp     = AB_VEC_INITIALIZER;
    AB_vec(vec2) tex_coords_tmp   = AB_VEC_INITIALIZER;
    AB_vec(vec3) normals_tmp      = AB_VEC_INITIALIZER;
    AB_vec(int) vertex_indices    = AB_VEC_INITIALIZER;
    AB_vec(int) tex_coord_indices = AB_VEC_INITIALIZER;
    AB_vec(int) normal_indices    = AB_VEC_INITIALIZER;

    /* Expect a lot of members */
    AB_vec_resize(&vertices_tmp,      256);
    AB_vec_resize(&tex_coords_tmp,    256);
    AB_vec_resize(&normals_tmp,       256);
    AB_vec_resize(&vertex_indices,    128);
    AB_vec_resize(&tex_coord_indices, 128);
    AB_vec_resize(&normal_indices,    128);

    /* Parse the input line-by-line */
    char *line = NULL;
    size_t n = 0;
    while (AB_GETLINE(&line, &n, infile) != -1) {
        float a, b, c;
        int ai, bi, ci, di, ei, fi, gi, hi, ii;
        if (sscanf(line, "v %f %f %f", &a, &b, &c) == 3) {
            /* Vertex location */
            vec3 *vert = AB_vec_pushp(&vertices_tmp);
            if (!vert)
                goto cleanup_err;
            (*vert)[0] = a, (*vert)[1] = b, (*vert)[2] = c;
        } else if (sscanf(line, "vt %f %f", &a, &b) == 2) {
            /* Vertex texture-coord */
            vec2 *coord = AB_vec_pushp(&tex_coords_tmp);
            if (!coord)
                goto cleanup_err;
            (*coord)[0] = a, (*coord)[1] = b;
        } else if (sscanf(line, "vn %f %f %f", &a, &b, &c) == 3) {
            /* Vertex normal */
            vec3 *norm = AB_vec_pushp(&normals_tmp);
            if (!norm)
                goto cleanup_err;
            (*norm)[0] = a, (*norm)[1] = b, (*norm)[2] = c;
        } else if (sscanf(line, "f %f %f %f", &a, &b, &c) == 3) {
            /* Triangle with location */
            int err;
            err = AB_vec_push(&vertex_indices, a); if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, b); if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, c); if (err) goto cleanup_err;
        } else if (sscanf(line, "f %d//%d %d//%d %d//%d",
                    &ai, &bi, &ci, &di, &ei, &fi) == 6) {
            /* Triangle with location and normal */
            int err;
            err = AB_vec_push(&vertex_indices, ai); if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, ci); if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, ei); if (err) goto cleanup_err;
            err = AB_vec_push(&normal_indices, bi); if (err) goto cleanup_err;
            err = AB_vec_push(&normal_indices, di); if (err) goto cleanup_err;
            err = AB_vec_push(&normal_indices, fi); if (err) goto cleanup_err;
        } else if (sscanf(line, "f %d/%d %d/%d %d/%d",
                    &ai, &bi, &ci, &di, &ei, &fi) == 6) {
            /* Triangle with location and texture-coord */
            int err;
            err = AB_vec_push(&vertex_indices, ai);    if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, ci);    if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, ei);    if (err) goto cleanup_err;
            err = AB_vec_push(&tex_coord_indices, bi); if (err) goto cleanup_err;
            err = AB_vec_push(&tex_coord_indices, di); if (err) goto cleanup_err;
            err = AB_vec_push(&tex_coord_indices, fi); if (err) goto cleanup_err;
        } else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &ai, &bi, &ci, &di, &ei, &fi, &gi, &hi, &ii) == 9) {
            /* Triangle with location, texture-coord and normal */
            int err;
            err = AB_vec_push(&vertex_indices, ai);    if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, di);    if (err) goto cleanup_err;
            err = AB_vec_push(&vertex_indices, gi);    if (err) goto cleanup_err;
            err = AB_vec_push(&tex_coord_indices, bi); if (err) goto cleanup_err;
            err = AB_vec_push(&tex_coord_indices, ei); if (err) goto cleanup_err;
            err = AB_vec_push(&tex_coord_indices, hi); if (err) goto cleanup_err;
            err = AB_vec_push(&normal_indices, ci);    if (err) goto cleanup_err;
            err = AB_vec_push(&normal_indices, fi);    if (err) goto cleanup_err;
            err = AB_vec_push(&normal_indices, ii);    if (err) goto cleanup_err;
        } else if (strncmp(line, "o ", 2) == 0) {
            if (out->name)
                free(out->name);
            out->name = AB_STRDUP(line + 2);
        }
    }
    free(line);

    /* Resize the output vectors to the correct size */
    AB_vec_resize(&out->vertices,   AB_vec_size(&vertex_indices));
    AB_vec_resize(&out->tex_coords, AB_vec_size(&tex_coord_indices));
    AB_vec_resize(&out->normals,    AB_vec_size(&normal_indices));

    /* Convert from .obj format to a usable one */
    for (int i = 0; i < AB_vec_size(&vertex_indices); i++) {
        int vert_idx = AB_vec_at(&vertex_indices, i);
        if (vert_idx < 0) {
            vert_idx = AB_vec_size(&tex_coord_indices) + vert_idx;
        }
        if (vert_idx >= 0 && vert_idx <= AB_vec_size(&vertices_tmp)) {
            vec3 *vertex = &AB_vec_at(&vertices_tmp, vert_idx - 1);
            vec3 *vertex0 = AB_vec_pushp(&out->vertices);
            memcpy(vertex0, vertex, sizeof(vec3));
        } else {
            AB_LOG_ERROR(
                    "[Objloader] Vertex index %u is beyond the number of vertices %u",
                    vert_idx, AB_vec_size(&vertices_tmp));
            goto cleanup_err;
        }
    }
    for (int i = 0; i < AB_vec_size(&tex_coord_indices); i++) {
        int tex_idx = AB_vec_at(&tex_coord_indices, i);
        if (tex_idx < 0) {
            tex_idx = AB_vec_size(&tex_coord_indices) + tex_idx;
        }
        if (tex_idx >= 0 && tex_idx <= AB_vec_size(&tex_coords_tmp)) {
            vec2 *tex_coord = &AB_vec_at(&tex_coords_tmp, tex_idx - 1);
            vec2 *tex_coord0 = AB_vec_pushp(&out->tex_coords);
            memcpy(tex_coord0, tex_coord, sizeof(vec2));
        } else {
            AB_LOG_ERROR(
                    "[Objloader] Vertex index %u is beyond the number of vertices %u",
                    tex_idx, AB_vec_size(&tex_coords_tmp));
            goto cleanup_err;
        }
    }
    for (int i = 0; i < AB_vec_size(&normal_indices); i++) {
        int norm_idx = AB_vec_at(&normal_indices, i);
        if (norm_idx < 0) {
            norm_idx = AB_vec_size(&tex_coord_indices) + norm_idx;
        }
        if (norm_idx >= 0 && norm_idx <= AB_vec_size(&normals_tmp)) {
            vec3 *norm = &AB_vec_at(&normals_tmp, norm_idx - 1);
            vec3 *norm0 = AB_vec_pushp(&out->normals);
            memcpy(norm0, norm, sizeof(vec3));
        } else {
            AB_LOG_ERROR(
                    "[Objloader] Normal index %u is beyond the number of normals %u",
                    norm_idx, AB_vec_size(&normals_tmp));
            goto cleanup_err;
        }
    }

    /* Cleanup tmp structures */
    AB_vec_destroy(&vertices_tmp);
    AB_vec_destroy(&tex_coords_tmp);
    AB_vec_destroy(&normals_tmp);
    AB_vec_destroy(&normal_indices);
    AB_vec_destroy(&vertex_indices);
    AB_vec_destroy(&tex_coord_indices);
    return 0;

cleanup_err:
    /* Cleanup tmp structures */
    AB_vec_destroy(&vertices_tmp);
    AB_vec_destroy(&tex_coords_tmp);
    AB_vec_destroy(&normals_tmp);
    AB_vec_destroy(&normal_indices);
    AB_vec_destroy(&vertex_indices);
    AB_vec_destroy(&tex_coord_indices);
    /* Cleanup input structures */
    AB_vec_destroy(&out->vertices);
    AB_vec_destroy(&out->tex_coords);
    AB_vec_destroy(&out->normals);
    free(out->name);
    return 1;

}
