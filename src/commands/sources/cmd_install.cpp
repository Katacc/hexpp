#include "commands/headers/cmd_install.h"

#include <iostream>
#include <string>

using namespace std;


void cmd_install::help() {

    cout << "Install packages from vcpkg.json" << endl;
    cout << "Usage:     hx++ install\n" << endl;

}

Error_code cmd_install::install_packages() {

    // Install packages
    try {
        string install = "vcpkg install";
        system(install.c_str());
        cout << "Packages succesfully installed!" << endl;
        return success;
    } catch (exception &e) {
        cerr << "Error installing packages: " << e.what() << endl;
        return failure;
    }

}