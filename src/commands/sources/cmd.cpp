#include "commands/headers/cmd.h"


// Replace windows filepath
std::string normalize_path(std::string path) {
    using namespace std;

    string normalized_path = path;

    for (int i = 0; i < normalized_path.size(); i++) {
        char letter = normalized_path.at(i);
        if (letter == 92) {
            normalized_path.replace(i, 1, "/");
        }
    }

    return normalized_path;
}

