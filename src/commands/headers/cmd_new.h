#ifndef CMD_NEW_H
#define CMD_NEW_H

#include "cmd.h"
#include "context.h"

using namespace std;


namespace cmd_new {


    void help();
    Error_code create_new(string project, Context &build_config);
    Error_code regen_config();

    static string project_name;

    Error_code write_main();
    Error_code write_test();
    Error_code write_module();
    Error_code write_CMakePresets(Context &build_config);
    Error_code write_CMakeUserPresets(Context &build_config);
    Error_code write_vscode_tasks();
    Error_code write_vscode_launch(Context &build_config);
    Error_code write_CMakeLists();
    Error_code write_gitignore();
    Error_code write_project_det();

}

#endif /* CMD_NEW_H */