#include "commands/headers/cmd_build.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdio>

using std::string, std::cout, std::endl, std::cerr;
namespace fs = std::filesystem;


void cmd_build::help() {

    cout << "Builds the project using CMake" << endl;
    cout << "Usage:     hx++ build [config] [preset]\n" << endl;
    cout << "   Options:" << endl;
    cout << "   [config]    -- If empty, use Debug config" << endl;
    cout << "       --debug" << endl;
    cout << "       --release" << endl;
    cout << "   [preset]    -- If empty, use configured default preset" << endl;
    cout << "       --default" << endl;
    cout << "       --custom" << endl;
    cout << "       --clang" << endl;
    cout << "       --vcpkg" << endl;

}

Error_code cmd_build::build(const Context &build_config) {

    cout << "Building project using CMake" << endl;
    cout << "preset: " << build_config.preset << endl;
    cout << "config: " << build_config.config << endl;

    string preset_command = "cmake --preset=" + build_config.preset;
    string build_command = "cmake --build " + build_config.BUILD_DIR + " --config " + build_config.config;

    fs::path dir = build_config.BUILD_DIR;
    if (!fs::exists(dir)) {
        try {
            fs::create_directory(build_config.BUILD_DIR);

        } catch (std::exception e) {
            cerr << "Error creating directory Build: " <<  e.what() << endl;
            return failure;
        }

        int config_result = system(preset_command.c_str());
        if (config_result != 0) {
            cerr << "Error: config command failed with exit code " << config_result << "\n";
            if(fs::exists(build_config.BUILD_DIR)) {
                fs::remove_all(build_config.BUILD_DIR);
                cerr << "Deleted build folder for fresh configuration..\n";
                cerr << "Make sure that your project.cfg contains name with correct syntax and your CMakeLists.txt is not corrupted\n";
                cerr << "name = project_name\n";
            }

            return failure;
        }
    }


    system(build_command.c_str());

    return success;
}
