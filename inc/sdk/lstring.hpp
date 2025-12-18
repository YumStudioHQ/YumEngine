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
  class String;

  class StringView : public containers::stringlookup<char> {
  public:
    using containers::stringlookup<char>::find;
    using containers::stringlookup<char>::rfind;

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

    inline StringView(const containers::stringlookup<char> &look_down_HAHAHAH)
      : containers::stringlookup<char>(look_down_HAHAHAH) {}

    StringView(const String&);

    StringView(const std::string&&) = delete;

    inline const char *ascii() const { return this->start; }
    inline String move() const;
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
      : containers::basic_string<char>() {
        list.foreach([this](char c) {
          this->append(c);
        });
      }

    inline String(const char *cstr)
      : containers::basic_string<char>(cstr, std::strlen(cstr)) {}

    inline String(const char *cstr, uint64_t len)
      : containers::basic_string<char>(cstr, len) {}

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

    inline const char *ascii() const {
      return this->start;
    }

    template <typename SearchKind>
    inline uint64_t find(const SearchKind &view) {
      return StringView(*this).find(view);
    }

    template <typename SearchKind>
    inline uint64_t rfind(const SearchKind &view) {
      return StringView(*this).rfind(view);
    }
  };

  inline String operator+(const StringView &vA, const StringView &vB) {
    String s(vA);
    s.append(vB);
    return s;
  }

  String StringView::move() const {
    return String(*this);
  }

  StringView::StringView(const String &s) 
    : containers::stringlookup<char>(s.head(), s.length()) {}
}

