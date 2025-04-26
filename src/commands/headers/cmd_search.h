
#ifndef CMD_SEARCH_H
#define CMD_SEARCH_H

#include "cmd.h"

namespace cmd_search {

    using std::string;

    void help();
    Error_code search_package(const string package_name);
}

#endif /* CMD_SEARCH_H */
