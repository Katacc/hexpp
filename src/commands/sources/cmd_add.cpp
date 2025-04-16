#include "commands/headers/cmd_add.h"

#include <iostream>
#include <string>

using namespace std;


void cmd_add::help() {
    using namespace std;

    cout << "Adds a package to the project using vcpkg (installs during build)" << endl;
    cout << "Usage:     hx++ add <package name>\n" << endl;
    cout << "Options:" << endl;
    cout << "   <packages ..>" << endl;

}

Error_code cmd_add::add_package(const string package_name) {

    string package = package_name;

    // Install package
    try {
        string install_string = "vcpkg add port " + package;
        // string install = "vcpkg install";
        system(install_string.c_str());
        // system(install.c_str());
        cout << "Package: " << package << " succesfully added" << endl;
        return success;
    } catch (exception &e) {
        cerr << "Error adding package: " << e.what() << endl;
        return failure;
    }

}
