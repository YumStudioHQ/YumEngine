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

namespace YumEngine {
  /**
   * @brief Base type of each YumEngine's types since V2.1.x minor fix.
   */
  class YumObject {
  private:
    mutable bool freed = false;

  protected:
    inline virtual void _free() const {}
  
  public:
    inline bool isfreed() const { return freed; }
    /**
     * @brief frees the current object.
     */
    inline void free() const {
      if (!freed) {
        freed = true;
        _free();
      }
    }
  };

  struct YumObjectReference {
    YumObject *object;
    mutable bool freed;
  };

  struct YumListElement {
    YumListElement     *parent;
    YumListElement     *child;
    YumListElement     *head;
    YumObjectReference *current;
  };
}