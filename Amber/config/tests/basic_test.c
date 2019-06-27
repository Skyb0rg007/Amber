#include <Amber/util/common.h>
#include <Amber/config/conf.h>

int main(int argc, char *argv[])
{
    AB_errno_t err;
    const char *filename;
    AB_ASSERT(argc == 2);
    
    filename = argv[1];

    err = AB_config_loadfile(filename);
    AB_ASSERT(!err);

    AB_ASSERT(!strcmp("..", AB_config_get_string(AB_CONFIG_DATAPATH)));
    AB_QUICK_LOG("datapath = %s", AB_config_get_string(AB_CONFIG_DATAPATH));

    AB_config_quit();
    return 0;
}
