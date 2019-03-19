#include <Amber/util/common.h>
#include <Amber/scripting/load_config.h>

#include <stdlib.h>

int main(int argc, char *argv[])
{
    AB_ASSERT(argc == 2);

    struct AB_config cfg;
    int err = AB_load_config(&cfg, argv[1]);
    if (err) {
        AB_QUICK_LOG("Failed!");
    } else {
        AB_QUICK_LOG("resource_dir = %s", cfg.resource_dir);
        free(cfg.resource_dir);
    }

    return 0;
}
