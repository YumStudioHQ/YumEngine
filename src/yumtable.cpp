/* yumtable.cpp */

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

#include "inc/glob.hpp"
#include "inc/yumdec.h"
#include "inc/yumtable.hpp"
#include "inc/pinlist.hpp"

extern "C" {
  YUM_OUTATR YumCTable *YumCTable_new(void) {
    auto table = new YumCTable();
    YumEngine::G_pinlist().pin(new YumEngine::YumObjectReference(YumEngine::YumObjectReference{
      .object = table,
      .freed = false,
    }));
    return table;
  }

  YUM_OUTATR void YumCTable_delete(const YumCTable *table) {
    table->free();
    delete table;
  }

  YUM_OUTATR YumVariant *YumCTable_at(const YumCTable *table, const YumVariant *key) {
    if (table && table->has_key(*key)) {
      auto val = table->at(*key);
      return new YumVariant(std::move(val));//WAIT.
    }
    return nullptr;
  }

  YUM_OUTATR int32_t YumCTable_hasKey(const YumCTable *table, const YumVariant *key) {
    if (table && table->has_key(*key)) return static_cast<int32_t>(table->has_key(*key));
    return 0;
  }

  YUM_OUTATR YumVector *YumCTable_keys(const YumCTable *table) {
    if (table) {
      return new YumEngine::Vector(std::move(table->keys()));
    }

    return nullptr;
  }

  YUM_OUTATR YumVector *YumCTable_values(const YumCTable *table) {
    if (table) {
      return new YumEngine::Vector(std::move(table->values()));
    }

    return nullptr;
  }
}