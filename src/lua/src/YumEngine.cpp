#include <string>
#include <memory>
#include <format>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <functional>
#include <unordered_map>

#include "inc/yum.hpp"

#include "engine/action.hpp"
#include "engine/envm.hpp"
#include "engine/proc.hpp"

inline std::string trim(std::string &str) {
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

int main(int argc, char *const argv[]) {
    (*G_out()) << std::format("YumEngine -- {}.{}.YumEngine.{}.{}.{}", YUM_ENGINE_VERSION_STUDIO, YUM_ENGINE_VERSION_BRANCH, YUM_ENGINE_VERSION_MAJOR,  YUM_ENGINE_VERSION_MINOR, YUM_ENGINE_VERSION_PATCH) << std::endl << std::endl;


    return 0;
}
