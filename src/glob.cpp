/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *********************************************************/

#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

#include "inc/yumdec.h"
#include "inc/pinlist.hpp"

std::shared_ptr<std::ostream> &G_out() {
  thread_local std::shared_ptr<std::ostream> os{&std::cout, [](void*){}};
  return os;
}

std::shared_ptr<std::ostream> &G_err() {
  thread_local std::shared_ptr<std::ostream> os{&std::cerr, [](void*){}};
  return os;
}

std::shared_ptr<std::istream> &G_in() {
  thread_local std::shared_ptr<std::istream> is{&std::cin, [](void*){}};
  return is;
}

class CallbackBuf : public std::streambuf {
  std::function<void(const std::string&)> _callback;
  std::string _buffer;

protected:
  int overflow(int c) override {
    if (c != EOF) {
      _buffer.push_back((char)c);
      if (c == '\n') {
        sync();
      }
    }
    return c;
  }

  int sync() override {
    if (!_buffer.empty()) {
      _callback(_buffer);
      _buffer.clear();
    }
    return 0;
  }

public:
  CallbackBuf(std::function<void(const std::string&)> cb)
    : _callback(std::move(cb)) {}
};

class CallbackBasedStream : public std::ostream {
  CallbackBuf _buf;

public:
  CallbackBasedStream(std::function<void(const std::string&)> cb)
    : std::ostream(&_buf), _buf(std::move(cb)) {}
};


extern "C" {
#ifndef YUM_CXX_API_NO_C_EXPORT
  YUM_OUTATR void Yum_open_G_out(const char *path) {
    (*G_out()).rdbuf(std::ofstream(path).rdbuf());
  }

  YUM_OUTATR void Yum_open_G_err(const char *path) {
    (*G_err()).rdbuf(std::ofstream(path).rdbuf());
  }

  YUM_OUTATR void Yum_open_G_in(const char *path) {
    (*G_in()).rdbuf(std::ifstream(path).rdbuf());
  }

  YUM_OUTATR void Yum_redirect_G_out(void (*cb)(const char *s)) {
    G_out() = std::make_shared<CallbackBasedStream>([cb](const std::string &s) {
      cb(s.c_str());
    });    
  }

  YUM_OUTATR void Yum_redirect_G_err(void (*cb)(const char *s)) {
    G_err() = std::make_shared<CallbackBasedStream>([cb](const std::string &s) {
      cb(s.c_str());
    });
  }
#endif // YUM_CXX_API_NO_C_EXPORT
}