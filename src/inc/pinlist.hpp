/* pinlist.hpp */

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
#include <stdexcept>

namespace YumEngine {
  extern bool is_engine_init();

  class pinlist {
  private:
    YumPinnable *pintab = nullptr;
    bool finalized = false;

  public:
    inline pinlist(bool isglob = false) {
      if (isglob) return;
      if (!is_engine_init())
        throw std::runtime_error("New object of type " + std::string(typeid(this).name()) + " when the engine isn't initialized!");
    }

    inline ~pinlist() {
      if (!finalized) {
        (*G_err()) << "yum: warning: one pinlist instance is being destroyed while not being finalized, finalizing" << std::endl;
        finalize();
      }
    }
    
    inline void finalize() {
      (*G_out()) << "yum: finalizing" << std::endl;
      uint64_t count = 0;
      
      if (pintab) {
        YumPinnable *it = pintab->org;

        while (it) {
          if (it->object) {
            (*G_out()) << "yum: ref<" << std::hex << it->object << std::setfill('0') << std::setw(16) << "> " << std::flush;
            if (it->object_freed) {
              (*G_out()) << "already freed" << std::endl;
            } else {
              (*G_out()) << " not freed. Origin: " << it->object->getorg() << ", freeing: " << std::flush;
              it->object->free();
              delete it->object;
              (*G_out()) << "ok" << std::endl;
              it->object_freed = true;
            }
            count++;
          } else (*G_out()) << "yum: one element skipped" << std::endl;

          it = it->org;
        }

        while (pintab) {
          YumPinnable* next = pintab->org;
          delete pintab;
          pintab = next;
        }
      } else {
        (*G_out()) << "yum: nothing to clean" << std::endl;
      }

      finalized = true;
      (*G_out()) << "yum: finalized " << std::dec << count << " resources" << std::endl;
    }

    inline void pin(YumObject *element) {
      if (!element) return;

      if (!pintab) {
        pintab = new YumPinnable(YumPinnable {
          .object = element,
          .org = nullptr
        });
        element->set_pin(pintab);
      } else {
        auto node = new YumPinnable(YumPinnable {
          .object = element,
          .org = pintab
        });

        pintab = node;
        element->set_pin(node);
      }
    }
  };
}