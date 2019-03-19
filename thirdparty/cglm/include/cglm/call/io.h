/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef cglmc_io_h
#define cglmc_io_h
#ifdef __cplusplus
extern "C" {
#endif

#include "../cglm.h"

CGLM_EXPORT
void
glmc_mat4_print(mat4   matrix,
                FILE * CGLM_RESTRICT ostream);

CGLM_EXPORT
void
glmc_mat3_print(mat3 matrix,
                FILE * CGLM_RESTRICT ostream);

CGLM_EXPORT
void
glmc_vec4_print(vec4 vec,
                FILE * CGLM_RESTRICT ostream);

CGLM_EXPORT
void
glmc_vec3_print(vec3 vec,
                FILE * CGLM_RESTRICT ostream);

CGLM_EXPORT
void
glmc_versor_print(versor vec,
                  FILE * CGLM_RESTRICT ostream);

#ifdef __cplusplus
}
#endif
#endif /* cglmc_io_h */
