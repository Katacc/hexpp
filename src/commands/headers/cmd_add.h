#ifndef CMD_ADD_H
#define CMD_ADD_H

#include "cmd.h"

using namespace std;


namespace cmd_add {

    void help();
    Error_code add_package(const string package_name);
}

#endif /* CMD_ADD_H */