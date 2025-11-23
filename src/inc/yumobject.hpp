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
  struct YumPinnable;

  /**
   * @brief Base type of each YumEngine's types since V2.1.x minor fix.
   */
  class YumObject {
  private:
    mutable bool freed = false;
    mutable YumPinnable *pinptr;

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

    inline YumPinnable *get_pin() const { return pinptr; }
    inline void set_pin(YumPinnable *pin) const { pinptr = pin; }
  };

  struct YumPinnable {
    YumObject *object;
    YumPinnable *child;
    YumPinnable *org; // This one points to the first element.
    mutable bool object_freed = false;
  };
}