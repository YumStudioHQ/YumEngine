/*************************************************************************************
 *                                                                                   *
 *               __   __               _____             _                           *
 *               \ \ / /   _ _ __ ___ | ____|_ __   __ _(_)_ __   ___                *
 *                \ V / | | | '_ ` _ \|  _| | '_ \ / _` | | '_ \ / _ \               *
 *                 | || |_| | | | | | | |___| | | | (_| | | | | |  __/               *
 *                 |_| \__,_|_| |_| |_|_____|_| |_|\__, |_|_| |_|\___|               *
 *                                                 |___/                             *
 *                                                                                   *
 *                                   By YumStudio                                    *
 *                                  Lead by モノエ.                                   * 
 *                                                                                   *
 *                                All rights reserved                                *
 *                            This file is free & open source,                       *
 *                             And covered by the MIT license                        *
 *                                                                                   *
 *                        https://github.com/YumStudioHQ/YumEngine                   *
 *                             https://github.com/YumStudioHQ                        *
 *                              https://github.com/wys-prog                          *
 *                                                                                   *
 *************************************************************************************/

#pragma once

#include "inc/types/containers/string.hpp"
#include "inc/types/containers/enumerable.hpp"
#include "inc/sdk/lbuffer.hpp"

#include <cstring>
#include <string>
#include <string_view>

namespace YumEngine::xV1::Sdk {
  class StringView : public containers::stringlookup<char> {
  public:
    inline StringView() 
      : containers::stringlookup<char>() {}
    inline StringView(const std::string &string) 
      : containers::stringlookup<char>(string.c_str(), string.size()) {}
    inline StringView(const std::string_view &view) 
      : containers::stringlookup<char>(view.data(), view.size()) {}
    inline StringView(const char *_ascii) 
      : containers::stringlookup<char>(_ascii, std::strlen(_ascii)) {}
    inline StringView(const char *_ascii, uint64_t size) 
      : containers::stringlookup<char>(_ascii, size) {}
    inline StringView(const containers::basic_string<char> &string) 
      : containers::stringlookup<char>(string.head(), string.length()) {}

    StringView(const std::string&&) = delete;

  };

  class String : public containers::basic_string<char> {
  public:
    inline String() {}
    inline String(const Buffer<char> &buff)
      : containers::basic_string<char>(buff.head(), buff.length()) {}

    inline String(const StringView &view) 
      : containers::basic_string<char>(view.head(), view.length()) {}
    
    inline String(const containers::span<char> &span) 
      : containers::basic_string<char>(span.head(), span.length()) {}
    
    inline String(const containers::list<char> &list)
      : containers::basic_string<char>(list.head(), list.length()) {}

    inline String(const char *cstr)
      : containers::basic_string<char>(cstr, std::strlen(cstr)) {}

    template <typename Enumerable>
    String(const Enumerable &enumerable)
      : containers::basic_string<char>(enumerable.head(), enumerable.length()) {}

    inline Buffer<char> tobuffer() const {
      return Buffer<char>(this->start, this->_size);
    }

    inline containers::memoryslice<char> tomemory() const {
      return containers::memoryslice<char>(this->start, this->_size);
    }

    inline containers::span<char> tospan() const {
      return containers::span<char>(this->start, this->_size);
    }

    inline String operator*(int times) const {
      if (times <= 0) {
        return String();
      }
      
      String result;
      result.realloc(this->_size * times);
      for (int i = 0; i < times; ++i) {
        result.append(*this);
      }
      return result;
    }

    inline String& operator*=(int times) {
      *this = (*this) * times;
      return *this;
    }
  };

  inline String operator+(const StringView &vA, const StringView &vB) {
    String s(vA);
    s.append(vB);
    return s;
  }
}

