#include <string>
#include <sstream>
#include <fstream>

#include "../lua/src/inc/engine_version.h"
#include "../lua/src/inc/variant.hpp"
#include "../lua/src/inc/vector.hpp"
#include "../lua/src/inc/glob.hpp"

#include "yurt.hpp"

using namespace Yumcxx;

class StreamRuntime : Yumcxx::YumRuntimeElement {
private:
  std::unordered_map<int64_t, std::shared_ptr<std::iostream>> handles;

public:
  StreamRuntime() {
    ns = "stream";
    table["new_streambuff"] = std::make_shared<ManagedCallback>([this](Vector args) -> Vector {
      int64_t uid = uid_next();
      handles[uid] = std::make_shared<std::stringstream>(std::stringstream());

      args.foreach([this, uid](Variant &v) {
        if (v.is_string()) (*handles[uid]) << v.as_string();
        else if (v.is_int()) (*handles[uid]) << v.as_int();
        else if (v.is_float()) (*handles[uid]) << v.as_float();
        else if (v.is_bool()) (*handles[uid]) << v.as_bool();
      });

      Vector rslt;
      rslt.append(Variant(uid));
      return rslt;
    });
  }
};