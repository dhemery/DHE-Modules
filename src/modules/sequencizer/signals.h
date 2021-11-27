#pragma once
#include "anchor.h"
#include "control-ids.h"
#include "interrupter.h"
#include "status.h"
#include "sustainer.h"
#include "trigger-mode.h"

#include "components/range.h"
#include "signals/basic.h"
#include "signals/curvature-signals.h"
#include "signals/duration-signals.h"
#include "signals/gain.h"
#include "signals/shapes.h"
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

template <typename TParam, typename TInput, typename TOutput, typename TLight,
          int N>
struct Signals {
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
    auto const range_id =
        value_of<VoltageRangeId>(params_[ParamId::LevelRange]);
    return Voltage::scale(rotation, range_id);
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
    return Curvature::scale(rotation);
  }

  auto duration(int step) const -> float {
    DurationRangeId range_id =
        value_of<DurationRangeId>(params_[ParamId::DurationRange]);
    auto const rotation = value_of(params_[ParamId::StepDuration + step]);
    auto const nominal_duration = Duration::scale(rotation, range_id);
    auto const multiplier =
        Gain::scale(rotation_of(params_[ParamId::DurationMultiplier],
                                inputs_[InputId::DurationMultiplierCV]));
    auto const scaled_duration = nominal_duration * multiplier;
    return cx::max(scaled_duration, ShortDuration::range.lower_bound());
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

  auto shape(int step) const -> Shape::Id {
    return value_of<Shape::Id>(params_[ParamId::StepShape + step]);
  }

private:
  using LightId = LightIds<N>;
  using ParamId = ParamIds<N>;

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
