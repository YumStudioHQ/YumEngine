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
      (*G_out()) << "yum: finalizing" << std::endl;
      YumListElement *it = top;
      uint64_t count = 0;
      std::vector<YumListElement*> toclean{};

      while (it != nullptr) {
        toclean.push_back(it);
        if (it->current) {
          (*G_out()) << "yum: ref<" << std::hex << it->current << "> ";
          if (it->current->freed) {
            (*G_out()) << "already freed" << std::endl;
          } else {
            (*G_out()) << " not freed, freeing" << std::endl;
            it->current->object->free();
          }
        } else {
          (*G_out()) << "yum: got bad ref..." << std::endl;
        }

        it = it->child;
        count++;
      }

      for (const auto &V : toclean) delete V;
      finalized = true;
      (*G_out()) << "yum: finalized " << std::dec << count << " resources" << std::endl;
    }

    inline void pin(YumObjectReference *element) {
      if (!element) return;

      YumListElement* newNode = new YumListElement({
        .child = nullptr,
        .current = element
      });

      if (!top) {
        top = newNode;
        return;
      }

      YumListElement* it = top;
      while (it->child != nullptr) {
        it = it->child;
      }

      it->child = newNode;
    }


  };


  pinlist &G_pinlist();
}