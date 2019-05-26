#pragma once

#include "Controls.h"
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>
#include <functional>

namespace dhe {
namespace duration {
  static Range constexpr shortRange{0.001F, 1.F};
  static Range constexpr mediumRange{0.01F, 10.F};
  static Range constexpr longRange{0.1F, 100.F};

  extern std::array<Range const *, 3> const ranges;

  /**
   * Returns the range for the given range switch position.
   * @param switchPosition the position of the range switch
   */
  auto range(int switchPosition) -> Range const *;

  /**
   * Creates a function that yields the duration (in seconds) selected by a
   * cv-modulated knob and a duration range switch.
   * @param knobId the ID of the duration knob param
   * @param cvId the ID of the control voltage input
   * @param switchId the ID of the duration range switch
   */
  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()>;

  /**
   * Creates a function that yields the duration (in seconds) selected by a knob
   * from a given range.
   * @param knobId the ID of the duration knob param
   * @param range the range from which to select the duration
   */
  auto withFixedRange(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()>;

  void configKnob(rack::engine::Module *module, int knobId, Range const & /*range*/,
                  std::string const &name = "Duration", float initialRotation = knob::centered);

  void configKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name = "Duration",
                  float initialRotation = knob::centered);

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Duration Range",
                    int initialPosition = 1);

} // namespace duration
} // namespace dhe
