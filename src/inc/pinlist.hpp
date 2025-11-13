/* gc.hpp */

/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE                         *
 *                                                       *
 *********************************************************/

#pragma once

/**
 * @file gc.hpp
 * 
 * @brief Internal Garbage Collector
 */

#include "glob.hpp"
#include "yumobject.hpp"

#include <vector>
#include <memory>
#include <cstdint>
#include <iomanip>

namespace YumEngine {
  class pinlist {
  private:
    YumListElement *top = nullptr;
    bool finalized = false;

  public:
    inline pinlist() {}
    inline ~pinlist() {
      if (!finalized) {
        (*G_err()) << "yum: warning: one pinlist instance is being destroyed while not being finalized, finalizing" << std::endl;
        finalize();
      }
    }
    
    inline void finalize() {
      YumListElement *it = top;

      while (it) {
        auto cur = it->current;
        if (cur && !cur->freed) {
          cur->object->free();
          (*G_out()) << "freeing ref<0x" << std::hex << std::setfill('0') << std::setw(16) << cur << std::endl;
          it = it->child;
          delete cur->object;
          delete cur;
        }
      }

      finalized = true;
    }

    inline void pin(YumObjectReference *element) {
      if (!element) { return; }
      if (top) {
        YumListElement *obj = new YumListElement(YumListElement{
          .parent = top,
          .child = nullptr,
          .head = top,
          .current = element,
        });

        top->child = obj;
      } else {
        top = new YumListElement(YumListElement{
          .parent = top,
          .child = nullptr,
          .head = nullptr,
          .current = element,
        });
      }
    }
  };
}