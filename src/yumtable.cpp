/* yumtable.cpp */

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

#include "inc/glob.hpp"
#include "inc/yumdec.h"
#include "inc/yumtable.hpp"
#include "inc/pinlist.hpp"
#include "inc/yumsys.hpp"

extern "C" {
#ifndef YUM_CXX_API_NO_C_EXPORT
  YUM_OUTATR YumCTable *YumCTable_new(void) {
    auto table = new YumCTable();
    YumEngine::G_yglob().get().pin(table);
    table->setorg(YumEngine::C_API);
    return table;
  }

  YUM_OUTATR void YumCTable_delete(const YumCTable *table) {
    table->free();
    delete table;
  }

  YUM_OUTATR YumVariant *YumCTable_at(const YumCTable *table, const YumVariant *key) {
    if (table && table->has_key(*key)) {
      auto val = table->at(*key);
      auto ptr = new YumVariant(std::move(val));
      YumEngine::G_yglob().get().pin(ptr);
      ptr->setorg(YumEngine::C_API_AT);

      return ptr;
    }
    return nullptr;
  }

  YUM_OUTATR int32_t YumCTable_hasKey(const YumCTable *table, const YumVariant *key) {
    if (table && table->has_key(*key)) return static_cast<int32_t>(table->has_key(*key));
    return 0;
  }

  YUM_OUTATR YumVector *YumCTable_keys(const YumCTable *table) {
    if (table) {
      auto ptr = new YumEngine::Vector(std::move(table->keys()));
      YumEngine::G_yglob().get().pin(ptr);
      return ptr;
    }

    return nullptr;
  }

  YUM_OUTATR YumVector *YumCTable_values(const YumCTable *table) {
    if (table) {
      auto ptr = new YumEngine::Vector(std::move(table->values()));
      YumEngine::G_yglob().get().pin(ptr);
      return ptr;
    }

    return nullptr;
  }

  YUM_OUTATR void YumCTable_set(YumCTable *table, const YumVariant *key, const YumVariant *value) {
    if (table && key && value) {
      (*table).set((*key), (*value));
    }
  }

  YUM_OUTATR uint64_t YumCTable_size(const YumCTable *table) {
    if (table) {
      return table->size();
    }

    return 0;
  }
#endif // YUM_CXX_API_NO_C_EXPORT
}