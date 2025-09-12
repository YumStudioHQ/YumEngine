#include <string>
#include <fstream>
#include <filesystem>

namespace fs {
  std::string absolute(const std::string &path) {
    return std::filesystem::absolute(path).lexically_normal();
  }

  namespace file {
    bool file_exists(const std::string &path) {
      return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    bool create_file(const std::string &path) {
      
    }
  }
}