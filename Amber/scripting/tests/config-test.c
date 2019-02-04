#include <Amber/util/common.h>
#include <Amber/scripting/load_config.h>

#include <stdlib.h>

int main(int argc, char *argv[])
{
    AB_ASSERT(argc == 2);

    struct AB_config cfg;
    int err = AB_load_config(&cfg, argv[1]);
    if (err) {
        AB_LOG_ERROR("Failed!");
    } else {
        AB_LOG_INFO("resource_dir = %s", cfg.resource_dir);
        free(cfg.resource_dir);
    }

    return 0;
}
