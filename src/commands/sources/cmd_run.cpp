#include "commands/headers/cmd_run.h"


#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

using std::string, std::cout, std::endl, std::cerr, std::exception;
namespace fs = std::filesystem;

void cmd_run::help() {
    using namespace std;
    cout << "Runs the project executable from build/" << endl;
    cout << "Usage:     hx++ run [config]\n" << endl;
    cout << "   Options:" << endl;
    cout << "       [config]    -- If empty, run debug" << endl;
    cout << "           --debug" << endl;
    cout << "           --release" << endl;

}

Error_code cmd_run::run(Context &build_config, string passthrough) {


    // Check if build directory exists
    if (!fs::exists(build_config.BUILD_DIR)) {
        cerr << "Error: build directory: " << build_config.BUILD_DIR << " does not exist!" << endl;
        return failure;
    }

    // Set extension to .exe if on windows
    string extension = "";
    #ifdef _WIN32
        extension = ".exe";
    #else
        extension = "";
    #endif

    string project_name = "";


    // Get the project name from project.cfg
    fs::path project_name_path("project.cfg");
    try {

        // Open the file
        std::ifstream project_file(project_name_path);

        // Get the name from the line name
        if (project_file.is_open()) {
            string line = {};
            while (getline(project_file, line)) {
                if (line.find("name") != string::npos) {
                    project_name = line;
                    break;
                }
            }
            project_file.close();
            // Parse the name starting from 7th character
            project_name = project_name.substr(7);
        } else {
            cout << "error opening project file..." << endl;
            project_file.close();
            return failure;
        }
    } catch (exception e) {
        cerr << "Error parsing the name from project.cfg, please check the file.\n";
        return failure;
    }

    // Add extension to project name
    project_name = project_name + extension;

    // Search for executable with the name of the project
    bool exe_found = false;
    for (const auto& entry : fs::directory_iterator(build_config.BUILD_DIR + "/" + build_config.config)) {
        string files = entry.path().string();

        if (files.contains(project_name)) {
            exe_found = true;
            cout << "Found executable: " << entry.path() << endl;

            string exec_command = "\"" + fs::absolute(files).string() + "\" " + passthrough;
            replace(exec_command.begin(), exec_command.end(), '\\', '/');
            cout << "Running: " << project_name << endl;
            cout << "----------------------------------------------" << endl;
            system(exec_command.c_str());
            return success;
        }
    }

    if (!exe_found) {
        cerr << "Error: No .exe files found in: " << build_config.BUILD_DIR << "/" << build_config.config << endl;
        cerr << "Did you build your project?\n";
        return failure;
    }

    return success;

}
