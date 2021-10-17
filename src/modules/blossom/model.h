#pragma once
#include "module.h"
#include "panel.h"

#include "rack.hpp"

namespace dhe {
namespace blossom {
rack::Model *model = rack::createModel<Module, Panel>("Blossom");
}
} // namespace dhe
