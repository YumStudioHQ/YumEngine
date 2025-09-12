#include <string>
#include <memory>
#include <format>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <functional>
#include <unordered_map>

#include "inc/yum.hpp"

inline std::string trim(std::string& str) {
    str.erase(str.find_last_not_of(' ') + 1);  // Suffixing spaces
    str.erase(0, str.find_first_not_of(' '));  // Prefixing spaces
    return str;
}

int main(int argc, char *const argv[]) {
    (*G_out()) << std::format("YumEngine -- {}.{}.YumEngine.{}.{}.{}",
        YUM_ENGINE_VERSION_STUDIO, YUM_ENGINE_VERSION_BRANCH, YUM_ENGINE_VERSION_MAJOR, 
        YUM_ENGINE_VERSION_MINOR, YUM_ENGINE_VERSION_PATCH) << std::endl << std::endl;

    throw std::runtime_error("PWAHAHAH YA THOUGHT THAT THIS APP WAS FINISHED !!!");

    return 0;
}

/*
    -rs String
    -rf File
    -live [--s String --f File..]
*/