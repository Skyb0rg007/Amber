
%include {
    #include <Amber/graphics/objloader.h>
    #include <Amber/util/vector.h>
    #include <Amber/util/common.h>
    #include <cglm/cglm.h>

    #include "objparse.h" /* Contains the terminal ids */
    #include "objlex.h"   /* Contains lexer function prototypes */

    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>

    /* Don't define Alloc or Free functions - parser is on stack */
    #define Objloader_ENGINEALWAYSONSTACK 1
    /* Don't use error recovery, no %parse_error */
    #define YYNOERRORRECOVERY 1

    struct AB_objloader_temp {
        char *name;
        int lineno, column;
        int has_err;
        AB_vec(vec3) vertices_tmp;
        AB_vec(vec2) tex_coords_tmp;
        AB_vec(vec3) normals_tmp;
        AB_vec(unsigned) vertex_indices, tex_coord_indices, normal_indices;
    };
}

%extra_argument { struct AB_objloader_temp *ast }

/********************
 * Error conditions
 *******************/
%syntax_error {
    AB_LOG_ERROR("[Objloader] Syntax error - [%d:%d]", ast->lineno, ast->column);
    ast->has_err = 1;
}
%stack_overflow {
    AB_LOG_ERROR("[Objloader] Parser stack overflow on line %d", ast->lineno);
    ast->has_err = 1;
}
%parse_accept {
    AB_LOG_INFO("[Objloader] Parsing succeeded!");
}

/*******************
 * Grammar
 ******************/
%name Objloader

// Tokens are 'unsigned', 'float', or 'char *'
// TODO: accept negative indices
%token_type { union AB_objloader_token }

%start_symbol obj

obj ::= obj_lines.

obj_lines ::= obj_lines obj_line_nl.
obj_lines ::= obj_line_nl.

obj_line_nl ::= NEWLINE.
obj_line_nl ::= obj_line NEWLINE.

obj_line ::= vertex.
obj_line ::= tex_coord.
obj_line ::= normal.
obj_line ::= index.
obj_line ::= object_name.
obj_line ::= smoothing.

// vertex - Where a point is in space
// Only support 3D points for now
vertex ::= GEO_VERTEX floating(A) floating(B) floating(C). {
    vec3 *vert = AB_vec_pushp(&ast->vertices_tmp); AB_ASSERT(vert);
    (*vert)[0] = A; (*vert)[1] = B; (*vert)[2] = C;
}

// tex_coord - What texture coordinates to use
// Only support 2D texture coordinates for now
tex_coord ::= TEXTURE_COORD floating(A) floating(B). {
    vec2 *coord = AB_vec_pushp(&ast->tex_coords_tmp); AB_ASSERT(coord);
    (*coord)[0] = A; (*coord)[1] = B;
}

// normal - Where the normal vertex is
// normalize the vector as it passes through
// Only support 3D normal vectors for now
normal ::= VERTEX_NORMAL floating(A) floating(B) floating(C). {
    vec3 *norm = AB_vec_pushp(&ast->normals_tmp); AB_ASSERT(norm);
    (*norm)[0] = A; (*norm)[1] = B; (*norm)[2] = C;
    glm_vec3_normalize(*norm);
}

// index - What coordinates to use
index ::= POLY_FACE v_index(A) v_index(B) v_index(C). {
    AB_vec_push(&ast->vertex_indices, A.v);
    AB_vec_push(&ast->vertex_indices, B.v);
    AB_vec_push(&ast->vertex_indices, C.v);
}
index ::= POLY_FACE vt_index(A) vt_index(B) vt_index(C). {
    AB_vec_push(&ast->vertex_indices, A.v);
    AB_vec_push(&ast->vertex_indices, B.v);
    AB_vec_push(&ast->vertex_indices, C.v);
    AB_vec_push(&ast->tex_coord_indices, A.t);
    AB_vec_push(&ast->tex_coord_indices, B.t);
    AB_vec_push(&ast->tex_coord_indices, C.t);
}
index ::= POLY_FACE vtn_index(A) vtn_index(B) vtn_index(C). {
    AB_vec_push(&ast->vertex_indices, A.v);
    AB_vec_push(&ast->vertex_indices, B.v);
    AB_vec_push(&ast->vertex_indices, C.v);
    AB_vec_push(&ast->tex_coord_indices, A.t);
    AB_vec_push(&ast->tex_coord_indices, B.t);
    AB_vec_push(&ast->tex_coord_indices, C.t);
    AB_vec_push(&ast->normal_indices, A.n);
    AB_vec_push(&ast->normal_indices, B.n);
    AB_vec_push(&ast->normal_indices, C.n);
}

// The object's name
object_name ::= NAMED_OBJECT OBJECT_NAME(A). {
    ast->name = A.string;
}

// Objects with only vertices
%type v_index {struct {unsigned v;}}
v_index(A) ::= NATURAL(B).
    { A.v = B.natural; }
// Objects with vertices and texture_coordinates
%type vt_index {struct {unsigned v, t;}}
vt_index(A) ::= NATURAL(B) SLASH NATURAL(C).
    { A.v = B.natural; A.t = C.natural; }
// Objects with vertices, texture_coordinates, and normals
%type vtn_index {struct {unsigned v, t, n;}}
vtn_index(A) ::= NATURAL(B) SLASH NATURAL(C) SLASH NATURAL(D).
    { A.v = B.natural; A.t = C.natural; A.n = D.natural; }

// floating - accept a floating point number or integer cast to a float
%type floating {float}
floating(A) ::= NUMBER(B).  { A = B.floating; }
floating(A) ::= NATURAL(B). { A = (float)B.natural; }

// Smoothing: TODO
smoothing ::= SMOOTHING_ENABLE.

%code {

    int AB_load_obj(FILE *infile, struct AB_mesh_info *out)
    {
        if (infile == NULL || out == NULL) {
            AB_LOG_ERROR("[Objloader] AB_load_obj() passed NULL parameter");
            return 1;
        }

        int ret = 0;

        /* Setup output structure */
        AB_vec_init(&out->vertices);
        AB_vec_init(&out->tex_coords);
        AB_vec_init(&out->normals);
        out->name = NULL;

        /* Setup lexer */
        yyscan_t scanner;
        objloader_lex_init(&scanner);
        objloader_set_in(infile, scanner);
        
        /* Setup parser on the stack */
        struct yyParser parser;
        ObjloaderInit(&parser);

        /* Lex and parse the obj file */
        int tok; 
        union AB_objloader_token val;
        struct AB_objloader_temp temp = {0};
        /* Expect a lot of members */
        AB_vec_resize(&temp.vertices_tmp,   256);
        AB_vec_resize(&temp.tex_coords_tmp, 256);
        AB_vec_resize(&temp.normals_tmp,    256);
        AB_vec_resize(&temp.vertex_indices,    128);
        AB_vec_resize(&temp.tex_coord_indices, 128);
        AB_vec_resize(&temp.normal_indices,    128);
        do {
            tok = objloader_lex(&val, scanner);
            temp.lineno = objloader_get_lineno(scanner);
            temp.column = objloader_get_column(scanner);
            Objloader(&parser, tok, val, &temp);
        } while (!temp.has_err && !feof(infile));

        /* Check for errors */
        if (temp.has_err) {
            ret = 1;
            goto cleanup;
        }

        /* Resize the output vectors to the correct size */
        AB_vec_resize(&out->vertices,   AB_vec_size(&temp.vertex_indices));
        AB_vec_resize(&out->tex_coords, AB_vec_size(&temp.tex_coord_indices));
        AB_vec_resize(&out->normals,    AB_vec_size(&temp.normal_indices));

        /* Convert from .obj format to a usable one */
        for (unsigned i = 0; i < AB_vec_size(&temp.vertex_indices); i++) {
            unsigned vert_idx = AB_vec_at(&temp.vertex_indices, i);
            if (vert_idx != 0 && vert_idx <= AB_vec_size(&temp.vertices_tmp)) {
                const vec3 *vertex = &AB_vec_at(&temp.vertices_tmp, vert_idx - 1);
                vec3 *vertex0 = AB_vec_pushp(&out->vertices);
                memcpy(vertex0, vertex, sizeof(vec3));
            } else {
                AB_LOG_ERROR(
                    "[Objloader] Vertex index %u is beyond the number of vertices %u",
                    vert_idx, AB_vec_size(&temp.vertices_tmp));
                ret = 1;
                goto cleanup;
            }
        }
        for (unsigned i = 0; i < AB_vec_size(&temp.tex_coord_indices); i++) {
            unsigned tex_idx = AB_vec_at(&temp.tex_coord_indices, i);
            if (tex_idx != 0 && tex_idx <= AB_vec_size(&temp.tex_coords_tmp)) {
                const vec2 *tex_coord = &AB_vec_at(&temp.tex_coords_tmp, tex_idx - 1);
                vec2 *tex_coord0 = AB_vec_pushp(&out->tex_coords);
                memcpy(tex_coord0, tex_coord, sizeof(vec2));
            } else {
                AB_LOG_ERROR(
                    "[Objloader] Vertex index %u is beyond the number of vertices %u",
                    tex_idx, AB_vec_size(&temp.tex_coords_tmp));
                ret = 1;
                goto cleanup;
            }
        }
        for (unsigned i = 0; i < AB_vec_size(&temp.normal_indices); i++) {
            unsigned norm_idx = AB_vec_at(&temp.normal_indices, i);
            if (norm_idx != 0 && norm_idx <= AB_vec_size(&temp.normals_tmp)) {
                const vec3 *norm = &AB_vec_at(&temp.normals_tmp, norm_idx - 1);
                vec3 *norm0 = AB_vec_pushp(&out->normals);
                memcpy(norm0, norm, sizeof(vec3));
            } else {
                AB_LOG_ERROR(
                    "[Objloader] Normal index %u is beyond the number of normals %u",
                    norm_idx, AB_vec_size(&temp.normals_tmp));
                ret = 1;
                goto cleanup;
            }
        }
        out->name = temp.name;
            
    cleanup:
        /* Cleanup tmp structure */
        AB_vec_destroy(&temp.vertices_tmp);
        AB_vec_destroy(&temp.tex_coords_tmp);
        AB_vec_destroy(&temp.normals_tmp);
        AB_vec_destroy(&temp.normal_indices);
        AB_vec_destroy(&temp.vertex_indices);
        AB_vec_destroy(&temp.tex_coord_indices);
        if (ret != 0)
            free(temp.name);
        /* Cleanup parser */
        ObjloaderFinalize(&parser);
        /* Cleanup lexer */
        objloader_lex_destroy(scanner);
        return ret;
    }

}
