#include <iostream>
#include <string>
#include <set>
#include <cstdlib>
#include <filesystem>
#include <fstream>

#include "context.h"
#include "commands/headers/cmd.h"
#include "commands/headers/cmd_new.h"
#include "commands/headers/cmd_add.h"
#include "commands/headers/cmd_build.h"
#include "commands/headers/cmd_run.h"
#include "commands/headers/cmd_install.h"


Context build_config;

// Prototypes
void help();
int parse_arguments(int &arc, char *argv[]);

void get_default_configs() {
    using std::string, std::exception, std::cout, std::cerr, std::endl;
    namespace fs = std::filesystem;

    string user_home = "";

    #ifdef _WIN32
        user_home = std::getenv("USERPROFILE");
    #else
        user_home = std::getenv("HOME");
    #endif

    string path_to_config = user_home + "/.config/hex++";

    // Normalize path
    replace(path_to_config.begin(), path_to_config.end(), '\\', '/');

    if (fs::exists(path_to_config)) {
        try {
            string config_file_string = path_to_config + "/config.ini";
            fs::path config_file_path(config_file_string);


            // Open config file
            std::ifstream config_file(config_file_path);

            string line = {};
            if (config_file.is_open()) {
                while(getline(config_file, line)) {
                    if (line.contains("CXX_COMPILER_PATH")) {
                        build_config.CXX_COMPILER_PATH = line.substr(20);
                    }
                    if (line.contains("C_COMPILER_PATH")) {
                        build_config.C_COMPILER_PATH = line.substr(18);
                    }
                    if (line.contains("C_DEBUGGER_PATH")) {
                        build_config.C_DEBUGGER_PATH = line.substr(18);
                    }
                    if (line.contains("default_preset")) {
                        build_config.preset = line.substr(17);
                    }
                }

            }
        } catch (exception e) {
            cerr << "Error: " << e.what() << endl;
        }
    }


}


int main(int argc, char *argv[]) {
    using namespace std;

    cout << "proj. Hex++  version: 1.0.2" << endl;
    cout << "---------------------------\n" << endl;

    get_default_configs();

    parse_arguments(argc, argv);

    return 0;
}

int parse_arguments(int &argc, char *argv[]) {

    set<string> commands_set = {};


    for (int i = 1; i < argc ; i++) {
        commands_set.insert(string(argv[i]));
    }

    // If the input parameters include --help, activate help switch
    bool help_switch = false;
    if (commands_set.contains("--help")) {
        help_switch = true;
    }


    int error_flag = 0;

    std::set<string> accepted_commands = {"new", "add", "build", "run", "install", "--help", "--default", "--vcpkg", "--custom", "--clang", "--release", "--debug"};
    for (const string &command : commands_set) {
        if (!accepted_commands.contains(command)) {
            error_flag = 1;
            break;
        }
    }


    if (argc < 2) {
        help();
        return 1;
    } else {



        string command = argv[1];

        // COMMAND NEW
        if (command == "new") {
            if (help_switch) {
                cmd_new::help();
                return 0;
            } else {

                if (argc == 3 && string(argv[2]) != "") {

                    string project_name = argv[2];
                    cmd_new::create_new(project_name, build_config);
                } else {
                    cmd_new::help();
                }
                return 0;
            }


        // COMMAND ADD
        } else if (command == "add") {
            if (help_switch) {
                cmd_add::help();
                return 0;
            } else {

                if (argc >= 3 && string(argv[2]) != "") {

                    string package_name = "";
                    for (int i = 2; i < argc; i++) {
                        package_name.append(argv[i]);
                        package_name.append(" ");
                    }
                    cmd_add::add_package(package_name);
                } else {
                    cmd_add::help();
                }
                return 0;
            }

        // COMMAND BUILD
        } else if (command == "build") {
            if (help_switch) {
                cmd_build::help();
                return 0;
            } else {

                if (error_flag == 1) {
                    cout << "ERROR: Unknown parameter(s) detected\n\n",
                    cmd_build::help();
                    return 1;
                }

                if (commands_set.contains("--debug")) {
                    build_config.config = "Debug";
                }
                if (commands_set.contains("--release")) {
                    build_config.config = "Release";
                }
                if (commands_set.contains("--default")) {
                    build_config.preset = "default";
                }
                if (commands_set.contains("--vcpkg")) {
                    build_config.preset = "vcpkg";
                }
                if (commands_set.contains("--custom")) {
                    build_config.preset = "custom";
                }
                if (commands_set.contains("--clang")) {
                    build_config.preset = "clang";
                }

                cmd_build::build(build_config);

                return 0;
            }


        // COMMAND RUN
        } else if (command == "run") {
            if (help_switch) {
                cmd_run::help();
                return 0;

            } else {

                if (error_flag == 1) {
                    cout << "ERROR: Unknown parameter(s) detected\n\n",
                    cmd_run::help();
                    return 1;
                }

                if (commands_set.contains("--debug")) {
                    build_config.config = "Debug";
                }
                if (commands_set.contains("--release")) {
                    build_config.config = "Release";
                }

                cmd_run::run(build_config);

                return 0;
            }

        } else if (command == "install") {
            if (help_switch) {
                cmd_install::help();

                return 0;
            } else {

                if (argc > 2) {
                    cout << "ERROR: Unknow parameter(s) detected\n\n";
                    cmd_install::help();
                    return 1;
                }

                cmd_install::install_packages();

                return 0;
            }

        } else {
            help();
            return 0;
        }
    }
}

void help() {
    using namespace std;

    cout << "Usage:     hx++ <command> <argument> [options]\n" << endl;
    cout << "Commands:" << endl;
    cout << "   new <project name>      -- Creates a new project" << endl;
    cout << "   add <packages ..>       -- Adds packages using vcpkg" << endl;
    cout << "   build [config] [preset] -- Builds project" << endl;
    cout << "   run [config]            -- Runs the executable" << endl;
    cout << "Options:" << endl;
    cout << "   --help                  -- Shows the help of specific command" << endl;
}

