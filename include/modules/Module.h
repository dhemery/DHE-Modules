#pragma once
#include "engine/Module.hpp"
#include "modules/controls/Inputs.h"

namespace dhe {
class Module : public rack::engine::Module {
public:
  inline auto modulated(int knobId, int cvId, int avId) -> float { return rotation(this, knobId, cvId, avId); }
};
} // namespace dhe
