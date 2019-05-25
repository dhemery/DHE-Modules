#pragma once
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>
#include <functional>
#include <string>

namespace dhe {

namespace attenuator {
  static constexpr auto range = Range{0.F, 1.F};
  void config(rack::engine::Module *module, int knobId, std::string const &knobName);
} // namespace attenuator

namespace attenuverter {
  static constexpr auto range = Range{-1.F, 1.F};
  void config(rack::engine::Module *module, int knobId, std::string const &knobName);
} // namespace attenuverter

namespace button {
  void config(rack::engine::Module *module, int buttonId, std::string const &buttonName,
              std::array<std::string, 2> const &stateNames, int initialState);
}

namespace control {
  namespace scale {
    /**
     * Creates a function that scales a rotation to the range supplied by the
     * given function.
     */
    auto toRange(std::function<Range const *()> const &range) -> std::function<float(float)>;

    /**
     * Creates a function that scales a rotation to the given range.
     */
    auto toRange(Range const &range) -> std::function<float(float)>;

  } // namespace scale

  namespace range {
    /**
     * Creates a function that uses a switch to select a range.
     * @tparam N the number of ranges in the array
     * @param switchId the ID of the switch to select the range
     * @param ranges an array of ranges to select from
     */
    template <int N>
    auto selection(rack::engine::Module *module, int switchId, std::array<Range const *, N> const &ranges)
        -> std::function<Range const *()>;
  } // namespace range

} // namespace control

namespace gain {
  static constexpr auto range = Range{0.F, 2.F};
  static constexpr auto invertibleRange = Range{-2.F, 2.F};

  void config(rack::engine::Module *module, int knobId, std::string const &knobName);
} // namespace gain

namespace knob {
  /**
   * Creates a function that returns the rotation of a knob.
   * @param knobId the ID of the knob param
   */
  auto rotation(rack::engine::Module *module, int knobId) -> std::function<float()>;

  /**
   * Creates a function that returns the modulated rotation of a knob. The amount of modulation is determined by the
   * voltage of a CV input.
   * @param knobId the ID of the knob param
   * @param cvId the ID of the CV input
   */
  auto rotation(rack::engine::Module *module, int knobId, int cvId) -> std::function<float()>;

  /**
   * Creates a function that returns the modulated rotation of a knob. The amount of modulation is determined by the
   * voltage of a CV input, multiplied by the value of an attenuverter.
   * @param knobId the ID of the knob param
   * @param cvId the ID of the CV input
   * @param avId the ID of the attenuverter param
   */
  auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId) -> std::function<float()>;

  /**
   * Creates a function that scales a rotation.
   */
  auto scaled(std::function<float()> const &rotation, std::function<float(float)> const &scale)
      -> std::function<float()>;

  /**
   * Creates a function that scales a tapered rotation
   */
  auto scaled(std::function<float()> const &rotation, std::function<float(float)> const &taper,
              std::function<float(float)> const &scale) -> std::function<float()>;

  void config(rack::engine::Module *module, int knobId, std::string const &knobName, std::string const &units,
              Range const &range);

  void configPercentage(rack::engine::Module *module, int knobId, std::string const &knobName, Range const &range);
} // namespace knob

namespace toggle {
  template <int N>
  void config(rack::engine::Module *module, int toggleId, std::string const &toggleName,
              std::array<std::string, N> const &stateNames, int initialState);
}

} // namespace dhe
