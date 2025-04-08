#include "commands/headers/cmd_new.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;


void cmd_new::help(){

    cout << "Creates a new project and needed files" << endl;
    cout << "Usage:     hx++ new <project name>\n" << endl;
    cout << "Options:" << endl;
    cout << "   <project name>" << endl;

}

Error_code cmd_new::create_new(string project, Context &build_config) {

    cmd_new::project_name = project;

    cout << "Creating folders..." << endl;
    fs::path nested = project_name + "/src/libraries";

    if (!fs::exists(project_name)) {
        try {
            if (fs::create_directories(nested)) {
                cout << "Directories created succesfully..." << endl;
                string vscode_folder = project_name + "/.vscode";
                fs::create_directory(vscode_folder);
            } else {
                cout << "Failed to create directories!!" << endl;
            }
        } catch (exception& e) {
            cerr << e.what() << endl;
            return failure;
        }
    } else {
        cout << "Folder allready exists..." << endl;
        return failure;
    }

    fs::path original_path = fs::current_path();
    fs::current_path(project_name);

    try {
        string vcpkg_command = "vcpkg new --application";
        cout << "Executing command: " << vcpkg_command << endl;
        system(vcpkg_command.c_str());
        cout << "vcpkg succesfully initialized\n" << endl;
    } catch (exception &e) {
        cerr << "Error executing vcpkg command: " << e.what() << endl;
    }

    fs::current_path(original_path);

    try {
        cout << "1. Generating src/main.cp" << endl;
        if (cmd_new::write_main() != success) {
            cerr << "Failed to write to main.cpp" << endl;
        }

        cout << "2. Generating src/libraries/test.h" << endl;
        if (cmd_new::write_test() != success) {
            cerr << "Failed to write to test.h" << endl;
        }

        // cout << "3. Generating src/modules/module.ixx" << endl;
        // if (cmd_new::write_module() != success) {
        //     cerr << "Failed to write to module.ixx" << endl;
        // }

        cout << "3. Generating CMakePresets.json" << endl;
        if (cmd_new::write_CMakePresets(build_config) != success) {
            cerr << "Failed to write to CMakePresets" << endl;
        }

        cout << "4. Generating CMakeUserPresets.json" << endl;
        if(cmd_new::write_CMakeUserPresets(build_config) != success) {
            cerr << "Failed to write to CMakeUserPresets" << endl;
        }

        cout << "5. Generating .vscode/tasks.json" << endl;
        if (cmd_new::write_vscode_tasks() != success) {
            cerr << "Failed to write to vscode/tasks" << endl;
        }

        cout << "6. Generating .vscode/launch.json" << endl;
        if (cmd_new::write_vscode_launch(build_config) != success) {
            cerr << "Failed to write to vscode/launch" << endl;
        }

        cout << "7. Generating CMakeLists.txt" << endl;
        if (cmd_new::write_CMakeLists() != success) {
            cerr << "Failed to write to CMakeLists.txt" << endl;
        }

        cout << "8. Generating .gitignore" << endl;
        if (cmd_new::write_gitignore() != success) {
            cerr << "Failed to write to .gitignore" << endl;
        }

        cout << "9. Generating project.cfg" << endl;
        if (cmd_new::write_project_det() != success) {
            cerr << "Failed to write to project.cfg" << endl;
        }

        cout << "Project: " << project_name << " files generated!\n" << endl;

        cout << "Next steps: \n";
        cout << "Open project foler \"cd " << project_name << "\"\n";
        cout << "Build the project \"hx++ build\"\n";
        cout << "Run the project \"hx++ run\"\n";
        return success;
    } catch (exception &e) {
        cerr << "Error generating project: " << e.what() << endl;
        return failure;
    }

}

Error_code cmd_new::regen_config() {

    return success;
}

Error_code cmd_new::write_main() {



    string main_path = project_name + "/src/main.cpp";

    ofstream main_file (main_path);
    if (main_file.is_open()) {
        main_file << "#include <iostream>"              << endl;
        main_file << ""                                 << endl;
        main_file << "#include \"test.h\""              << endl;
        main_file << ""                                 << endl;
        main_file << "int main() {"                     << endl;
        main_file << "    using namespace std;"         << endl;
        main_file << ""                                 << endl;
        main_file << "    cout << \"Test\" << endl;"    << endl;
        main_file << "    test();"                      << endl;
        main_file << ""                                 << endl;
        main_file << "    return 0;"                    << endl;
        main_file << "}"                                << endl;
        main_file.close();
    } else {
        cout << "Unable to open the file..." << endl;
        return failure;
    }
    return success;

}

Error_code cmd_new::write_test() {

    string test_path = project_name + "/src/libraries/test.h";

    ofstream test_file (test_path);
    if (test_file.is_open()) {
        test_file << "#ifndef TEST_H"                                           << endl;
        test_file << "#define TEST_H"                                           << endl;
        test_file << "#include <iostream>"                                      << endl;
        test_file << ""                                                         << endl;
        test_file << "void test() {"                                            << endl;
        test_file << "    std::cout << \"Test function called!\" << std::endl;" << endl;
        test_file << ""                                                         << endl;
        test_file << "}"                                                        << endl;
        test_file << "#endif /* TEST_H */"                                      << endl;
        test_file.close();
    } else {
        cout << "Unable to open the file..." << endl;
        return failure;
    }
    return success;

}

Error_code cmd_new::write_module() {

    string module_path = project_name + "/src/modules/module.ixx";

    ofstream file (module_path);
    if (file.is_open()) {
        file << "module;" << endl;
        file << "# This is and example, can delete this!" << endl;
        file << "#include <iostream>" << endl;
        file << "export module def_module;" << endl;
        file << "" << endl;
        file << "" << endl;
        file << "export namespace def_module {" << endl;
        file << "   void print() {" << endl;
        file << "       std::cout << \"Test from modules/module!\" << std::endl;" << endl;
        file << "   }" << endl;
        file << "}" << endl;
        file.close();
    } else {
        cout << "Unable to open file.." << endl;
        return failure;
    }
    return success;
}

Error_code cmd_new::write_CMakePresets(Context &build_config) {

    // Define the path for the CMakePresets.json file
    string presets_path = project_name + "/CMakePresets.json";

    // Open the file for writing
    ofstream presets_file(presets_path);
    if (presets_file.is_open()) {
        // Write the JSON structure
        presets_file << "{" << endl;
        presets_file << "  \"version\": 2," << endl;
        presets_file << "  \"configurePresets\": [" << endl;
        presets_file << "    {" << endl;
        presets_file << "      \"name\": \"vcpkg\"," << endl;
        presets_file << "      \"generator\": \"Ninja Multi-Config\"," << endl;
        presets_file << "      \"binaryDir\": \"${sourceDir}/build\"," << endl;
        presets_file << "      \"cacheVariables\": {" << endl;
        presets_file << "        \"CMAKE_TOOLCHAIN_FILE\": \"$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake\"" << endl;
        presets_file << "      }" << endl;
        presets_file << "    }" << endl;
        presets_file << "  ]" << endl;
        presets_file << "}" << endl;

        // Close the file
        presets_file.close();
        return success;
    } else {
        cerr << "Unable to open file: " << presets_path << endl;
        return failure;
    }

}

Error_code cmd_new::write_CMakeUserPresets(Context &build_config) {
    using namespace std;

    // Define the path for the CMakeUserPresets.json file
    string user_presets_path = project_name + "/CMakeUserPresets.json";

    // Open the file for writing
    ofstream user_presets_file(user_presets_path);
    if (user_presets_file.is_open()) {
        // Write the JSON structure
        user_presets_file << "{" << endl;
        user_presets_file << "  \"version\": 2," << endl;
        user_presets_file << "  \"configurePresets\": [" << endl;
        user_presets_file << "    {" << endl;
        user_presets_file << "      \"name\": \"default\"," << endl;
        user_presets_file << "      \"inherits\": \"vcpkg\"," << endl;
        user_presets_file << "      \"environment\": {" << endl;
        user_presets_file << "          \"VCPKG_ROOT\": \"$env{HOME}/vcpkg\"," << endl;
        user_presets_file << "          \"CC\": \"gcc\"," << endl;
        user_presets_file << "          \"CXX\": \"g++\"" << endl;
        user_presets_file << "" << endl;
        user_presets_file << "      }" << endl;
        user_presets_file << "    }," << endl;
        user_presets_file << "    {" << endl;
        user_presets_file << "      \"name\": \"clang\"," << endl;
        user_presets_file << "      \"inherits\": \"vcpkg\"," << endl;
        user_presets_file << "      \"environment\": {" << endl;
        user_presets_file << "          \"VCPKG_ROOT\": \"$env{HOME}/vcpkg\"," << endl;
        user_presets_file << "          \"CC\": \"clang\"," << endl;
        user_presets_file << "          \"CXX\": \"clang++\"" << endl;
        user_presets_file << "" << endl;
        user_presets_file << "      }" << endl;
        user_presets_file << "    }," << endl;
        user_presets_file << "    {" << endl;
        user_presets_file << "      \"name\": \"custom\"," << endl;
        user_presets_file << "      \"inherits\": \"vcpkg\"," << endl;
        user_presets_file << "      \"environment\": {" << endl;
        user_presets_file << "          \"VCPKG_ROOT\": \"$env{HOME}/vcpkg\"," << endl;
        user_presets_file << "          \"CC\":" << "\"" << build_config.C_COMPILER_PATH <<  "\"," << endl;
        user_presets_file << "          \"CXX\":" << "\"" << build_config.CXX_COMPILER_PATH <<  "\"" << endl;
        user_presets_file << "" << endl;
        user_presets_file << "      }" << endl;
        user_presets_file << "    }" << endl;
        user_presets_file << "  ]" << endl;
        user_presets_file << "}" << endl;

        // Close the file
        user_presets_file.close();
        return success;
    } else {
        cerr << "Unable to open file: " << user_presets_path << endl;
        return failure;
    }
}


Error_code cmd_new::write_vscode_tasks() {
    // Define the path for the tasks.json file
    string tasks_path = project_name + "/.vscode/tasks.json";

    // Open the file for writing
    ofstream tasks_file(tasks_path);
    if (tasks_file.is_open()) {
        // Write the JSON structure
        tasks_file << "{" << endl;
        tasks_file << "    \"version\": \"2.0.0\"," << endl;
        tasks_file << "    \"tasks\": [" << endl;
        tasks_file << "        {" << endl;
        tasks_file << "            \"label\": \"CMake Configure\"," << endl;
        tasks_file << "            \"type\": \"shell\"," << endl;
        tasks_file << "            \"command\": \"cmake\"," << endl;
        tasks_file << "            \"args\": [" << endl;
        tasks_file << "                \"--preset=default\"" << endl;
        tasks_file << "            ]," << endl;
        tasks_file << "            \"problemMatcher\": []," << endl;
        tasks_file << "            \"group\": {" << endl;
        tasks_file << "                \"kind\": \"build\"," << endl;
        tasks_file << "                \"isDefault\": false" << endl;
        tasks_file << "            }" << endl;
        tasks_file << "        }," << endl;
        tasks_file << "        {" << endl;
        tasks_file << "            \"label\": \"CMake Build\"," << endl;
        tasks_file << "            \"type\": \"shell\"," << endl;
        tasks_file << "            \"command\": \"cmake\"," << endl;
        tasks_file << "            \"args\": [" << endl;
        tasks_file << "                \"--build\"," << endl;
        tasks_file << "                \"build\"" << endl;
        tasks_file << "            ]," << endl;
        tasks_file << "            \"problemMatcher\": \"$gcc\"," << endl;
        tasks_file << "            \"group\": {" << endl;
        tasks_file << "                \"kind\": \"build\"," << endl;
        tasks_file << "                \"isDefault\": true" << endl;
        tasks_file << "            }," << endl;
        tasks_file << "            \"dependsOn\": [\"CMake Configure\"]" << endl;
        tasks_file << "        }" << endl;
        tasks_file << "    ]" << endl;
        tasks_file << "}" << endl;

        // Close the file
        tasks_file.close();
        return success;
    } else {
        cerr << "Unable to open file: " << tasks_path << endl;
        return failure;
    }
}

Error_code cmd_new::write_vscode_launch(Context &build_config) {
    // Define the path for the launch.json file
    string launch_path = project_name + "/.vscode/launch.json";

    // Open the file for writing
    ofstream launch_file(launch_path);
    if (launch_file.is_open()) {
        // Write the JSON structure
        launch_file << "{" << endl;
        launch_file << "    \"version\": \"0.2.0\"," << endl;
        launch_file << "    \"configurations\": [" << endl;
        launch_file << "        {" << endl;
        launch_file << "            \"name\": \"Debug " << project_name << "\"," << endl;
        launch_file << "            \"type\": \"cppdbg\"," << endl;
        launch_file << "            \"request\": \"launch\"," << endl;
        launch_file << "            \"program\": \"${workspaceFolder}/build/" << project_name << ".exe\"," << endl;
        launch_file << "            \"args\": []," << endl;
        launch_file << "            \"stopAtEntry\": false," << endl;
        launch_file << "            \"cwd\": \"${workspaceFolder}\"," << endl;
        launch_file << "            \"environment\": []," << endl;
        launch_file << "            \"externalConsole\": false," << endl;
        launch_file << "            \"MIMode\": \"gdb\"," << endl;
        launch_file << "            \"miDebuggerPath\": " << "\"" << build_config.C_DEBUGGER_PATH << "\"" << "," << endl;
        launch_file << "            \"setupCommands\": [" << endl;
        launch_file << "                {" << endl;
        launch_file << "                    \"description\": \"Enable pretty-printing for gdb\"," << endl;
        launch_file << "                    \"text\": \"-enable-pretty-printing\"," << endl;
        launch_file << "                    \"ignoreFailures\": true" << endl;
        launch_file << "                }" << endl;
        launch_file << "            ]," << endl;
        launch_file << "            \"preLaunchTask\": \"CMake Build\"" << endl;
        launch_file << "        }" << endl;
        launch_file << "    ]" << endl;
        launch_file << "}" << endl;

        // Close the file
        launch_file.close();
        return success;
    } else {
        cerr << "Unable to open file: " << launch_path << endl;
        return failure;
    }
}

Error_code cmd_new::write_CMakeLists() {
    // Define the path for the CMakeLists.txt file
    string cmake_path = project_name + "/CMakeLists.txt";


    // Open the file for writing
    ofstream cmake_file(cmake_path);
    if (cmake_file.is_open()) {
        // Write the CMakeLists.txt content
        cmake_file << "cmake_minimum_required(VERSION 4.0)" << endl;
        cmake_file << endl;
        cmake_file << "configure_file(${CMAKE_SOURCE_DIR}/project.cfg ${CMAKE_BINARY_DIR}/project.cfg COPYONLY)" << endl;
        cmake_file << "file(STRINGS ${CMAKE_SOURCE_DIR}/project.cfg config_lines)" << endl;
        cmake_file << "" << endl;
        cmake_file << "foreach(line IN LISTS config_lines)" << endl;
        cmake_file << "     if(line MATCHES \"^name =\")" << endl;
        cmake_file << "         string(LENGTH \"name = \" prefix_len)" << endl;
        cmake_file << "         string(SUBSTRING \"${line}\" ${prefix_len} -1 PROJ_NAME)" << endl;
        cmake_file << "" << endl;
        cmake_file << "     endif()" << endl;
        cmake_file << "     if(line MATCHES \"^version =\")" << endl;
        cmake_file << "         string(LENGTH \"version = \" prefix_len_ver)" << endl;
        cmake_file << "         string(SUBSTRING \"${line}\" ${prefix_len_ver} -1 PROJ_VERSION)" << endl;
        cmake_file << "" << endl;
        cmake_file << "     endif()" << endl;
        cmake_file << "     if(line MATCHES \"^CXX_version =\")" << endl;
        cmake_file << "         string(LENGTH \"CXX_version = \" prefix_len_cxx)" << endl;
        cmake_file << "         string(SUBSTRING \"${line}\" ${prefix_len_cxx} -1 cxx_version)" << endl;
        cmake_file << "" << endl;
        cmake_file << "     endif()" << endl;
        cmake_file << "endforeach()" << endl;
        cmake_file << endl;
        cmake_file << "project(${PROJ_NAME} VERSION ${PROJ_VERSION})" << endl;
        cmake_file << endl;
        cmake_file << "# Static libraries" << endl;
        cmake_file << "set(CMAKE_FIND_LIBRARY_SUFFIXES \".a\")" << endl;
        cmake_file << "set(BUILD_SHARED_LIBS OFF)" << endl;
        cmake_file << "set(CMAKE_EXE_LINKER_FLAGS \"-static\")" << endl;
        cmake_file << endl;
        cmake_file << "# C++ version" << endl;
        cmake_file << "set(CMAKE_CXX_STANDARD ${cxx_version})" << endl;
        cmake_file << "set(CMAKE_CXX_STANDARD_REQUIRED True)" << endl;
        cmake_file << "set(CMAKE_COLOR_DIAGNOSTICS ON)" << endl;
        cmake_file << "" << endl;
        cmake_file << "# Debug options" << endl;
        cmake_file << "# Uncomment to enable address sanitizing (DOESNT WORK ON WINDOWS)" << endl;
        cmake_file << "#set(SANITIZER_FLAGS \"-fsanitize=address\")" << endl;
        cmake_file << "#add_compile_options(${SANITIZER_FLAGS})" << endl;
        cmake_file << "#add_link_options(${SANITIZER_FLAGS})" << endl;
        cmake_file << "" << endl;
        cmake_file << "" << endl;
        cmake_file << "# Determine the executable extension based on the operating system" << endl;
        cmake_file << "if(WIN32)" << endl;
        cmake_file << "set(EXECUTABLE_EXTENSION \".exe\")" << endl;
        cmake_file << "else()" << endl;
        cmake_file << "set(EXECUTABLE_EXTENSION \"\")" << endl;
        cmake_file << "endif()" << endl;
        cmake_file << "" << endl;
        cmake_file << "# (EXAMPLE)Find all .cpp files in src/libraries " << endl;
        cmake_file << "file(GLOB_RECURSE cpp_sources CONFIGURE_DEPENDS \"${PROJECT_SOURCE_DIR}/src/*.cpp\")" << endl;
        cmake_file << "file(GLOB_RECURSE module_files CONFIGURE_DEPENDS \"${PROJECT_SOURCE_DIR}/src/*.ixx\")" << endl;
        cmake_file << endl;
        cmake_file << endl;
        cmake_file << endl;
        cmake_file << endl;
        cmake_file << "# Target" << endl;
        cmake_file << "add_executable(${PROJECT_NAME} " << endl;
        cmake_file << "                     # .cpp files listed here or the variables containing them" << endl;
        cmake_file << "                     ${cpp_sources}" << endl;
        cmake_file << "                )" << endl;
        cmake_file << endl;
        cmake_file << "target_sources(${PROJECT_NAME}" << endl;
        cmake_file << "    PRIVATE" << endl;
        cmake_file << "        FILE_SET cxx_modules TYPE CXX_MODULES FILES" << endl;
        cmake_file << "            ${module_files}" << endl;
        cmake_file << ")" << endl;
        cmake_file << "" << endl;
        cmake_file << endl;
        cmake_file << "# Includes" << endl;
        cmake_file << "target_include_directories(${PROJECT_NAME} PRIVATE" << endl;
        cmake_file << "                            \"${PROJECT_BINARY_DIR}\"" << endl;
        cmake_file << "                            \"${PROJECT_SOURCE_DIR}\"" << endl;
        cmake_file << "                            \"${PROJECT_SOURCE_DIR}/src\"" << endl;
        cmake_file << "                            \"${PROJECT_SOURCE_DIR}/src/libraries\"" << endl;
        cmake_file << "                            \"${PROJECT_SOURCE_DIR}/src/modules\"" << endl;
        cmake_file << "                            )" << endl;
        cmake_file << endl;
        cmake_file << "# EXAMPLE OF ADDING A PACKAGE" << endl;
        cmake_file << "# Packages -------------" << endl;
        cmake_file << "# find_package(SDL3 CONFIG REQUIRED)" << endl;
        cmake_file << endl;
        cmake_file << "# SDL (EXAMPLE)" << endl;
        cmake_file << "target_link_libraries(${PROJECT_NAME} PRIVATE " << endl;
        cmake_file << "                                        # SDL3::SDL3" << endl;
        cmake_file << "                                        # Add packages here" << endl;
        cmake_file << "                                        )" << endl;
        cmake_file << endl;
        cmake_file << "# end of packages -----" << endl;
        cmake_file << endl;

        // Close the file
        cmake_file.close();
        return success;
    } else {
        cerr << "Unable to open file: " << cmake_path << endl;
        return failure;
    }
}

Error_code cmd_new::write_gitignore() {

    string path = project_name + "/.gitignore";

    ofstream gitignore_file(path);
    if (gitignore_file.is_open()) {
        gitignore_file << "build/" << endl;
        gitignore_file << ".vscode/.ropeproject/" << endl;
        gitignore_file << ".vscode/ipch/" << endl;
        gitignore_file << "compile_commands.json" << endl;
        gitignore_file << "*.obj" << endl;
        gitignore_file << "*.exe" << endl;
        gitignore_file << "*.ilk" << endl;
        gitignore_file << "*.pdb" << endl;

        gitignore_file.close();
        return success;
    } else {
        cerr << "Unable to open file: " << path << endl;
        return failure;
    }
}

// project.cfg
Error_code cmd_new::write_project_det() {

    string path = project_name + "/project.cfg";

    ofstream file(path);
    if (file.is_open()) {
        file << "[project]" << endl;
        file << "name = " << project_name << endl;
        file << "version = 1.0" << endl;
        file << "" << endl;
        file << "[CMake]" << endl;
        file << "CXX_version = 23" << endl;
        file << "" << endl;
        file.close();
        return success;
    } else {
        return failure;
    }
}