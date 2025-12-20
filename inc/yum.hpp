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

#include "inc/yumem.hpp"
#include "inc/types/base/callbacks.h"
#include "inc/types/base/types.h"
#include "inc/types/base/vardef.h"
#include "inc/types/containers/enumerable.hpp"
#include "inc/types/containers/memoryslice.hpp"
#include "inc/types/containers/span.hpp"
#include "inc/types/containers/string.hpp"
#include "inc/types/system/exception.hpp"
#include "inc/types/variant.hpp"
#include "inc/types/state.hpp"
#include "inc/utils/ystringutils.hpp"
#include "inc/version/engine_version.h"

namespace YumEngine {
  namespace YumEngineSdkContent {
    using namespace YumEngine::xV1;
    using namespace YumEngine::xV1::containers;
    using namespace YumEngine::xV1::Sdk;
  }

  namespace Sdk = YumEngineSdkContent;
  namespace ApiSdk = YumEngineSdkContent;
  namespace Api = YumEngineSdkContent;
  // Or whatever people call it !
}
