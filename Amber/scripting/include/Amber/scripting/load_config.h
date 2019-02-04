#ifndef AMBER_SCRIPTING_LOAD_CONFIG_H
#define AMBER_SCRIPTING_LOAD_CONFIG_H

#include <stdio.h>

struct AB_config {
    char *resource_dir;
};

int AB_load_config(struct AB_config *cfg, const char *file);

#endif /* AMBER_SCRIPTING_LOAD_CONFIG_H */
