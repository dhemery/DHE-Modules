#include <engine/Module.hpp>

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

namespace level {

const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

auto toSelectedRange(rack::engine::Module *module, int switchId)
    -> std::function<float(float)> {
  using namespace control;
  auto const selectedRange =
      range::selection<2>(module, switchId, level::ranges);
  return control::scale::toRange(selectedRange);
}

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId);
  return knob::scaled(rotation, scale::toRange(range));
}

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId);
  return knob::scaled(rotation, toSelectedRange(module, switchId));
}

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int avId, int switchId) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId, avId);
  return knob::scaled(rotation, toSelectedRange(module, switchId));
}

} // namespace level
} // namespace dhe
