/** @file conf.h
 *
 * Implementation of global configuration for Amber, loaded from a Lua file
 *
 */
#ifndef AMBER_CONFIG_CONF_H
#define AMBER_CONFIG_CONF_H

#include <Amber/util/common.h>

enum AB_config {
    AB_CONFIG_DATAPATH,
    AB_CONFIG_CONFIGPATH,
    AB_CONFIG_CACHEPATH,
    AB_CONFIG_VERBOSITY
};

AB_errno_t AB_config_loadfile(const char *filename);
const char *AB_config_get_string(enum AB_config conf);
unsigned AB_config_get_unsigned(enum AB_config conf);
void AB_config_quit(void);

#endif /* AMBER_CONFIG_CONF_H */
