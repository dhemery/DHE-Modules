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
   * @param knobId the ID of the level knob param
   * @param cvId the ID of the control voltage input
   * @param switchId the ID of the level range switch
   */
  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()>;

  /**
   * Creates a function that yields the level (in volts) selected by a modulated knob and a level range switch. The
   * amount of modulation is determined by the voltage of a CV input, multiplied by the value of an attenuverter.
   * @param knobId the ID of the level knob param
   * @param cvId the ID of the control voltage input
   * @param avId the ID of the attenuverter param
   * @param switchId the ID of the level range switch
   */
  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float()>;

  /**
   * Creates a function that yields the level (in volts) selected by a knob from the unipolar range.
   * @param knobId the ID of the level knob param
   * @param range the range from which to select a voltage
   */
  auto withUnipolarRange(rack::engine::Module *module, int knobId) -> std::function<float()>;

  void configKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name = "Level",
                  float initialRotation = knob::centered);

  void configKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name = "Level",
                  float initialRotation = knob::centered);

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Level Range",
                    int initialState = 1);
} // namespace level
} // namespace dhe
