#include "commands/headers/cmd_search.h"

#include <iostream>
#include <string>

using std::string, std::cerr, std::cout, std::endl;


void cmd_search::help() {

    cout << "Searches for package from vcpkg" << endl;
    cout << "Usage:     hx++ search <package name>\n" << endl;
    cout << "Options:" << endl;
    cout << "   <package>" << endl;   

}

Error_code cmd_search::search_package(const string package_name) {

    string package = package_name;

    try {
        string search_string = "vcpkg search " + package;

        system(search_string.c_str());

        return success;
    }
    catch (const std::exception &e) {
        cerr << "Error when searching: " << e.what() << endl;
        return failure;
    }
}
