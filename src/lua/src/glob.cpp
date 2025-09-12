#include <memory>
#include <iostream>

std::shared_ptr<std::ostream> &G_out() {
  static std::shared_ptr<std::ostream> os{&std::cout, [](void*){}};
  return os;
}

std::shared_ptr<std::ostream> &G_err() {
  static std::shared_ptr<std::ostream> os{&std::cerr, [](void*){}};
  return os;
}