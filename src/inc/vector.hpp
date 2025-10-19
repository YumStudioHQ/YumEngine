#pragma once

/* vector.hpp — Container */

/* using std::vector<T> with T = Yumcxx::Variant */
#include <vector>
#include <functional>

/* Include Variant type */
#include "variant.hpp"

namespace Yumcxx {
  /*  That's a simple Vector implementation,
   ** Because that class will be used in C/C++ bridges.
   ** To make it easier, and minimal, only 'essential' 
   ** methods are present. */
  class Vector {
  private:
    std::vector<Variant> internal;

  public:
    Vector();
    ~Vector();
    void append(const Variant&);
    void pop();
    void clear();
    int64_t size() const;
    Variant &at(int64_t);
    const Variant &at(int64_t) const;
    /* //! (FOR NOW!) : C++-only function! */
    void foreach(const std::function<void(const Variant&)>&) const;
    void foreach(const std::function<void(Variant&)>&);
  };
}

using YumVector = Yumcxx::Vector;