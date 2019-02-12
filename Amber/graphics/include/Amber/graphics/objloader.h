#ifndef AMBER_GRAPHICS_OBJLOADER_H
#define AMBER_GRAPHICS_OBJLOADER_H

#include <Amber/util/vector.h>
#include <cglm/cglm.h>
#include <stdio.h>

struct AB_mesh_info {
    char *name;
    AB_vec(vec3) vertices;
    AB_vec(vec2) tex_coords;
    AB_vec(vec3) normals;
};

int AB_load_obj(FILE *infile, struct AB_mesh_info *out);

#endif /* AMBER_GRAPHICS_OBJLOADER_H */
