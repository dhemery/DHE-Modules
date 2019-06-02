#pragma once
#include "Inputs.h"
#include "modules/components/Taper.h"
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>
#include <functional>
#include <string>

namespace dhe {

template <typename T, int N>
auto selectorFunction(rack::engine::Module const *module, int switchId, std::array<T, N> const &items)
    -> std::function<T()> {
  return [module, switchId, items]() -> T { return selected<T, N>(module, switchId, items); };
}

template <int N>
auto rangeSelectorFunction(rack::engine::Module const *module, int switchId, std::array<Range const *, N> const &ranges)
    -> std::function<Range const *()> {
  return selectorFunction<Range const *, N>(module, switchId, ranges);
}
} // namespace dhe
