#pragma once
#include "modules/components/Taper.h"
#include "util/Range.h"

#include <array>
#include <engine/Module.hpp>
#include <functional>
#include <string>

namespace dhe {

namespace attenuator {
  extern Range const range;

  /**
   * Configures the param and display for an attenuator knob.
   */
  void config(rack::engine::Module *module, int knobId, std::string const &knobName);
} // namespace attenuator

namespace attenuverter {
  extern Range const range;

  /**
   * Configures the param and display for an attenuverter knob.
   */
  void config(rack::engine::Module *module, int knobId, std::string const &knobName);
} // namespace attenuverter

namespace button {

  /**
   * Configures the param and display for a momentary button.
   */
  void config(rack::engine::Module *module, int buttonId, std::string const &buttonName,
              std::array<std::string, 2> const &stateNames, int initialState);

  /**
   * Creates a function that returns the state of a button.
   */
  auto state(rack::engine::Module *module, int buttonId) -> std::function<bool()>;
} // namespace button

namespace gain {
  extern Range const range;

  /**
   * Configures the param and display for a gain knob.
   */
  void config(rack::engine::Module *module, int knobId, std::string const &knobName);
} // namespace gain

namespace input {
  /**
   * Creates a function that returns whether an input is active.
   */
  auto isConnected(rack::engine::Module *module, int inputId) -> std::function<bool()>;

  /**
   * Creates a function that returns whether an input is high (above 1 volt).
   */
  auto isHigh(rack::engine::Module *module, int inputId) -> std::function<bool()>;

  /**
   * Creates a function that returns whether an input is high (above 1 volt) or a button is pressed.
   */
  auto isHigh(rack::engine::Module *module, int inputId, int buttonId) -> std::function<bool()>;
} // namespace input

namespace knob {
  extern Range const rotationRange;
  static auto constexpr centered = 0.5F;

  /**
   * Configures the param and display for a knob with a fixed, linear range.
   */
  void config(rack::engine::Module *module, int knobId, std::string const &knobName, std::string const &units,
              Range const &range, float initialRotation = centered);

  /**
   * Configures the param and display for a knob with a fixed, linear range, displayed as a percentage.
   */
  void configPercentage(rack::engine::Module *module, int knobId, std::string const &knobName, Range const &range);

  /**
   * Creates a function that returns the rotation of a knob.
   */
  auto rotation(rack::engine::Module *module, int knobId) -> std::function<float()>;

  /**
   * Creates a function that returns the modulated rotation of a knob. The amount of modulation is determined by the
   * voltage of a CV input.
   */
  auto rotation(rack::engine::Module *module, int knobId, int cvId) -> std::function<float()>;
  /**
   * Creates a function that returns the modulated rotation of a knob. The amount of modulation is determined by the
   * voltage of a CV input, multiplied by the value of an attenuverter.
   */
  auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId) -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, scaled to the given range.
   */
  auto scaled(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, modulated and scaled to the given range. The amount of
   * modulation is determined by the voltage of a CV input, multiplied by the value of an attenuverter.
   */
  auto scaled(rack::engine::Module *module, int knobId, int cvId, int avId, Range const &range)
      -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, modulated scaled to the supplied range. The amount of
   * modulation is determined by the voltage of a CV input.
   */
  auto scaled(rack::engine::Module *module, int knobId, int cvId, std::function<Range const *()> const &range)
      -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, modulated scaled to the supplied range. The amount of
   * modulation is determined by the voltage of a CV input, multiplied by the value of an attenuverter.
   */
  auto scaled(rack::engine::Module *module, int knobId, int cvId, int avId, std::function<Range const *()> const &range)
      -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, tapered and scaled to the given range.
   */
  auto taperedAndScaled(rack::engine::Module *module, int knobId, taper::FixedTaper const &taper, Range const &range)
      -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, modulated, tapered, and scaled to the given range. The
   * amount of modulation is determined by the voltage of a CV input, multiplied by the value of an attenuverter.
   */
  auto taperedAndScaled(rack::engine::Module *module, int knobId, int cvId, int avId, taper::FixedTaper const &taper,
                        Range const &range) -> std::function<float()>;

  /**
   * Creates a function that returns the rotation of a knob, modulated, tapered, and scaled to the supplied range. The
   * amount of modulation is determined by the voltage of a CV input.
   */
  auto taperedAndScaled(rack::engine::Module *module, int knobId, int cvId, taper::FixedTaper const &taper,
                        std::function<Range const *()> const &range) -> std::function<float()>;

} // namespace knob

namespace selection {
  /**
   * Creates a function that returns the item selected by a switch from a given array.
   */
  template <typename T, int N>
  auto of(rack::engine::Module *module, int switchId, std::array<T, N> const &items) -> std::function<T()> {
    return [module, switchId, items]() -> T {
      auto const selection = static_cast<int>(module->params[switchId].getValue());
      return items[selection];
    };
  }
} // namespace selection

namespace range {
  /**
   * Creates a function that returns the range selected by a switch.
   */
  template <int N>
  auto selected(rack::engine::Module *module, int switchId, std::array<Range const *, N> const &ranges)
      -> std::function<Range const *()> {
    return selection::of<Range const *, N>(module, switchId, ranges);
  }
} // namespace range

namespace toggle {
  /**
   * Configures the param and display for a toggle that represents a sequence of states.
   */
  template <int N>
  void config(rack::engine::Module *module, int toggleId, std::string const &toggleName,
              std::array<std::string, N> const &stateNames, int initialState);
} // namespace toggle

} // namespace dhe
