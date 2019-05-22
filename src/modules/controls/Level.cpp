#include <engine/Module.hpp>

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

namespace level {

const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId);
  auto const toRange = scale::toRange(range);
  return knob::scaled(rotation, toRange);
}

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId);
  auto const selectedRange =
      range::selection<2>(module, switchId, level::ranges);
  auto const toRange = scale::toRange(selectedRange);
  return knob::scaled(rotation, toRange);
}

} // namespace level
} // namespace dhe
