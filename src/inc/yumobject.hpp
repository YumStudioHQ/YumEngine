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
   * @brief represents the origin of a YumObject object
   */
  enum YumObjectOrigin : uint8_t {
    UNKNOWN,
    FUNCTION_CALL,
    INTERNAL_CXX,
    C_API,
    C_API_AT,
  };

  /**
   * @brief Base type of each YumEngine's types since V2.1.x minor fix.
   */
  class YumObject {
  private:
    mutable bool freed = false;
    mutable YumPinnable *pinptr;
    YumObjectOrigin origin = UNKNOWN;

  protected:
    inline virtual void _free() const {}
  
  public:
    inline std::string getorg() const {
      switch (origin) {
        case UNKNOWN: return "unknown";
        case FUNCTION_CALL: return "Lua function call";
        case INTERNAL_CXX: return "Internal C++";
        case C_API: return "C API";
        default: return "unknown";
      }
    }

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
    inline YumObject() {}
    inline YumObject(YumObjectOrigin org) 
      : origin(org) {}
    inline void setorg(YumObjectOrigin org) { origin = org; }

    inline virtual ~YumObject() {}
  };

  struct YumPinnable {
    YumObject *object;
    YumPinnable *org;
    mutable bool object_freed = false;
  };
}