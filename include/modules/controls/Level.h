#pragma once

#include "Controls.h"
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>

namespace dhe {
namespace level {
  extern Range const bipolarRange;
  extern Range const unipolarRange;

  extern std::array<Range const *, 2> const ranges;

  /**
   * Creates a function that yields the level (in volts) selected by a modulated knob and a level range switch. The
   * amount of modulation is determined by the voltage of a CV input.
   */
  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()>;

  /**
   * Creates a function that yields the level (in volts) selected by a modulated knob and a level range switch. The
   * amount of modulation is determined by the voltage of a CV input, multiplied by the value of an attenuverter.
   */
  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float()>;

  /**
   * Creates a function that yields the level (in volts) selected by a knob from the unipolar range.
   */
  auto withUnipolarRange(rack::engine::Module *module, int knobId) -> std::function<float()>;

  /**
   * Configures the param and display for a level knob with a fixed range.
   */
  void configKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name = "Level",
                  float initialRotation = knob::centered);

  /**
   * Configures the param and display for a level knob with a range selected by a switch.
   */
  void configKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name = "Level",
                  float initialRotation = knob::centered);

  /**
   * Configures the param and display for a duration range switch.
   */
  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Level Range",
                    int initialState = 1);
} // namespace level
} // namespace dhe
