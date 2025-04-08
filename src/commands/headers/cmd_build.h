#ifndef CMD_BUILD_H
#define CMD_BUILD_H

#include "cmd.h"
#include "context.h"

namespace cmd_build {

    void help();
    Error_code build(const Context &build_config);

}


#endif /* CMD_BUILD_H */