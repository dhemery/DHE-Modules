#pragma once
#include "widgets/dimensions.h"

#include "rack.hpp"

namespace dhe {

struct Attenuator {
  static constexpr auto range = Range{0.F, 1.F};
  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 1.F) {
    module->configParam(id, 0.F, 1.F, rotation, name, "%", 0.F, 100.F, 0.F);
  }
};

struct Attenuverter {
  static constexpr auto range = Range{-1.F, 1.F};
  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 1.F) {
    module->configParam(id, 0.F, 1.F, rotation, name, "%", -100.F, 100.F, 0.F);
  }
};
} // namespace dhe
