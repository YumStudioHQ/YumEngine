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

#include "inc/types/state.hpp"
#include "inc/types/containers/memoryslice.hpp"
#include "inc/types/variant.hpp"

namespace YumEngine::xV1::Sdk {
  template <typename T>
  class Buffer : public containers::memoryslice<T> {
  public:
    Buffer() {}
    Buffer(const T *from, uint64_t len, bool copy = false)
      : containers::memoryslice<T>(from, len, copy) {}
    
    Buffer(const containers::memoryslice<T> &slc) 
      : containers::memoryslice<T>(slc) {}
    
    Buffer(const containers::memoryslice<T> &slc, uint64_t len) 
      : containers::memoryslice<T>(slc, len) {}
    
    Buffer(uint64_t len) 
      : containers::memoryslice<T>(len) {}
    
    Buffer(const containers::list<T> &li, bool copy = false) 
      : containers::memoryslice<T>(li.data(), li.length(), copy) {}

    Buffer<T> &join(const Buffer<T> &buff) {
      uint64_t nsize = this->_length + buff.length();
      T *nbuff = new T[nsize];

      for (uint64_t i = 0; i < this->_length; i++) nbuff[i] = this->start[i];
      for (uint64_t i = this->_length; i < nsize; i++) nbuff[i] = buff.copy(i - this->_length);

      if (this->owns) delete[] this->start;
      this->owns = true;
      this->readonly = false;
      this->start = nbuff;
      this->_length = nsize;
      return *this;
    }

    Buffer<T> &join(const containers::list<Buffer<T>> &buffers) {
      for (uint64_t i = 0; i < buffers.length(); i++) this->join(buffers._enumerable_at_const(i));
      return *this;
    }

    Buffer<T> &join(const containers::memoryslice<Buffer<T>> &buffers) {
      for (uint64_t i = 0; i < buffers.length(); i++) this->join(buffers[i]);
      return *this;
    }
  };
}