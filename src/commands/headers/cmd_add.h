#ifndef CMD_ADD_H
#define CMD_ADD_H

#include "cmd.h"

namespace cmd_add {

    using std::string;

    void help();
    Error_code add_package(const string package_name);
}

#endif /* CMD_ADD_H */
