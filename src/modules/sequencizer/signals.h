#pragma once
#include "advancement.h"
#include "anchor.h"
#include "control-ids.h"
#include "status.h"

#include "components/range.h"
#include "signals/basic.h"
#include "signals/curvature.h"
#include "signals/durations.h"
#include "signals/gain.h"
#include "signals/voltages.h"

#include <vector>

namespace dhe {
namespace sequencizer {

// Skew the progress::brightness ratio so that the "remaining" light stays
// fully lit for a little while during early progress, and the "completed"
// light reaches fully lit a little while before progress is complete.
static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <typename P, typename I>
static inline auto duration(P const &duration_knob, P const &range_switch,
                            P const &multipler_knob, I const &multiplier_cv)
    -> float {
  static auto constexpr minimum_duration = ShortDuration::range().lower_bound();
  auto const nominal_duration =
      Durations::seconds(value_of(duration_knob), value_of(range_switch));
  auto const multiplier_rotation = rotation_of(multipler_knob, multiplier_cv);
  auto const nominal_multiplier = Gain::range().scale(multiplier_rotation);
  auto const clamped_multiplier = Gain::range().clamp(nominal_multiplier);
  auto const scaled_duration = nominal_duration * clamped_multiplier;
  auto const safe_duration = cx::max(minimum_duration, scaled_duration);
  return safe_duration;
}

template <typename TParam, typename TInput, typename TOutput, typename TLight,
          int N>
class Signals {
  using InputId = InputIds<N>;
  using LightId = LightIds<N>;
  using OutputId = OutputIds;
  using ParamId = ParamIds<N>;

public:
  Signals(std::vector<TInput> &inputs, std::vector<TParam> &params,
          std::vector<TOutput> &outputs, std::vector<TLight> &lights)
      : params_{params}, inputs_{inputs}, outputs_{outputs}, lights_{lights} {}

  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    auto const base = type == AnchorType::Start ? ParamId::StepStartAnchorMode
                                                : ParamId::StepEndAnchorMode;
    return value_of<AnchorMode>(params_[base + step]);
  }

  auto anchor_level(AnchorType type, int step) const -> float {
    auto const base = type == AnchorType::Start ? ParamId::StepStartAnchorLevel
                                                : ParamId::StepEndAnchorLevel;
    auto const rotation =
        rotation_of(params_[base + step], inputs_[InputId::LevelAttenuationCV],
                    params_[ParamId::LevelMultiplier]);
    auto const range = value_of<VoltageRange>(params_[ParamId::LevelRange]);
    return VoltageRanges::volts(rotation, range);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Start ? ParamId::StepStartAnchorSource
                                                : ParamId::StepEndAnchorSource;
    return value_of<AnchorSource>(params_[base + step]);
  }

  auto completion_mode(int step) const -> SustainMode {
    return value_of<SustainMode>(params_[ParamId::StepSustainMode + step]);
  }

  auto curvature(int step) const -> float {
    auto const rotation = rotation_of(params_[ParamId::StepCurvature + step]);
    return Curvature::curvature(rotation);
  }

  auto duration(int step) const -> float {
    return sequencizer::duration(params_[ParamId::StepDuration + step],
                                 params_[ParamId::DurationRange],
                                 params_[ParamId::DurationMultiplier],
                                 inputs_[InputId::DurationMultiplierCV]);
  }

  auto gate() const -> bool {
    return is_high(inputs_[InputId::Gate]) ||
           is_pressed(params_[ParamId::Gate]);
  }

  auto in_a() const -> float { return voltage_at(inputs_[InputId::InA]); }

  auto in_b() const -> float { return voltage_at(inputs_[InputId::InB]); }

  auto in_c() const -> float { return voltage_at(inputs_[InputId::InC]); }

  auto interrupt_mode(int step) const -> InterruptMode {
    return value_of<InterruptMode>(params_[ParamId::StepInterruptMode + step]);
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[ParamId::StepEnabled + step]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[ParamId::Loop]) ||
           is_high(inputs_[InputId::Loop]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[InputId::Reset]) ||
           is_pressed(params_[ParamId::Reset]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[ParamId::Run]) || is_high(inputs_[InputId::Run]);
  }

  auto output() const -> float { return voltage_at(outputs_[OutputId::Out]); }

  void output(float voltage) { outputs_[OutputId::Out].setVoltage(voltage); }

  auto selection_start() const -> int {
    return value_of<int>(params_[ParamId::SelectionStart]);
  }

  auto selection_length() const -> int {
    return value_of<int>(params_[ParamId::SelectionLength]);
  }

  auto trigger_mode(int step) const -> TriggerMode {
    return value_of<TriggerMode>(params_[ParamId::StepTriggerMode + step]);
  }

  void show_curving(bool curving) {
    outputs_[OutputId::IsCurving].setVoltage(curving ? 10.F : 0.F);
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  void show_sequence_event(bool event) {
    outputs_[OutputId::SequenceEventPulse].setVoltage(event ? 10.F : 0.F);
  }

  void show_step_event(bool event) {
    outputs_[OutputId::StepEventPulse].setVoltage(event ? 10.F : 0.F);
  }

  void show_step_status(int step, StepStatus status) {
    outputs_[OutputId::StepNumber].setVoltage(static_cast<float>(step + 1) *
                                              10.F / static_cast<float>(N));
    switch (status) {
    case StepStatus::Generating:
      outputs_[OutputId::IsCurving].setVoltage(10.F);
      outputs_[OutputId::IsSustaining].setVoltage(0.F);
      break;
    case StepStatus::Sustaining:
      outputs_[OutputId::IsCurving].setVoltage(0.F);
      outputs_[OutputId::IsSustaining].setVoltage(10.F);
      break;
    default:
      outputs_[OutputId::IsCurving].setVoltage(0.F);
      outputs_[OutputId::IsSustaining].setVoltage(0.F);
    }
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const shape = value_of<Shape>(params_[ParamId::StepShape + step]);
    return Shapes::taper(shape);
  }

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = LightId::StepProgress + step + step;
    auto const remaining_light = completed_light + 1;
    lights_[completed_light].setBrightness(completed_brightness);
    lights_[remaining_light].setBrightness(remaining_brightness);
  }
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
  std::vector<TLight> &lights_;
};
} // namespace sequencizer

} // namespace dhe
