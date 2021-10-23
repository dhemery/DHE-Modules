#pragma once

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
struct LabeledInts {
  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name,
                            std::vector<std::string> const &labels,
                            float initial = 0) {
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(initial);
    module->configSwitch(id, 0.F, max, default_value, name, labels);
  }
};
} // namespace dhe
