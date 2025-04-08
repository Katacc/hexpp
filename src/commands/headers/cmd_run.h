#ifndef CMD_RUN_H
#define CMD_RUN_H

#include "cmd.h"
#include "context.h"



namespace cmd_run {

    void help();
    Error_code run(Context &build_config);
}


#endif /* CMD_BUILD_H */