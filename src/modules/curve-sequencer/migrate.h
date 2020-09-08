#pragma once

#include "anchor.h"
#include "components/cxmath.h"
#include "controls.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curve_sequencer {
static auto constexpr module_json_version = 1;

template <int N> class PresetV0 {
  using Module = rack::engine::Module;

  // Params whose meanings changed from json v0 to v1. We must translate the
  // values of these params whenever we load a v0 preset.
  enum class V0Param {
    StepAdvanceMode = ParamIds<N>::StepTriggerMode,    // Values: AdvanceMode
    StepGenerateMode = ParamIds<N>::StepInterruptMode, // Values: GenerateMode
  };

  enum class V0GenerateMode {
    Curve,   // Start: Sample OUT; End: Track LEVEL
    Hold,    // Start: Sample OUT; End: Sample Out; Int: Ignore; At End: Advance
    Sustain, // Start: Sample OUT; End: Sample Out; Int: Advance; At End:
    // Sustain
    Input, // Start: Track IN; End: Track IN
    Chase, // Start: Sample OUT; End: Track IN
    Level, // Start: Track LEVEL; End: Track LEVEL
  };

  enum class V0AdvanceMode {
    // Unused after v0. Translate this value to:
    // - Int: Ignore
    // - At End: Advance
    // We can leave the param value itself at 0. In v1 this translates to
    // GateRises, which is a reasonable default. And besides, the choice of
    // trigger mode has no effect on patch behavior, because the Int and At End
    // settings leave the step ignoring all triggers.
    TimerExpires,
    // To update the rest of the values:
    // - Trigger: v0::AdvanceMode::X -> v1::TriggerMode::X
    // - Int: Advance
    // - At End: Advance
    GateRises,
    GateFalls,
    GateChanges,
    GateIsHigh,
    GateIsLow
  };

  static auto constexpr v0_advance_mode_base =
      static_cast<int>(V0Param::StepAdvanceMode);
  static auto constexpr v0_generate_mode_base =
      static_cast<int>(V0Param::StepGenerateMode);

  static auto constexpr completion_mode_base =
      static_cast<int>(ParamIds<N>::StepCompletionMode);
  static auto constexpr enable_step_base =
      static_cast<int>(ParamIds<N>::EnableStep);
  static auto constexpr end_level_base =
      static_cast<int>(ParamIds<N>::StepEndLevel);
  static auto constexpr end_mode_base =
      static_cast<int>(ParamIds<N>::StepEndAnchorMode);
  static auto constexpr end_source_base =
      static_cast<int>(ParamIds<N>::StepEndAnchorSource);
  static auto constexpr interrupt_mode_base =
      static_cast<int>(ParamIds<N>::StepInterruptMode);
  static auto constexpr start_level_base =
      static_cast<int>(ParamIds<N>::StepStartLevel);
  static auto constexpr start_mode_base =
      static_cast<int>(ParamIds<N>::StepStartAnchorMode);
  static auto constexpr start_source_base =
      static_cast<int>(ParamIds<N>::StepStartAnchorSource);
  static auto constexpr trigger_mode_base =
      static_cast<int>(ParamIds<N>::StepTriggerMode);

  static auto constexpr sample_mode_f = static_cast<float>(AnchorMode::Sample);
  static auto constexpr track_mode_f = static_cast<float>(AnchorMode::Track);
  static auto constexpr in_source_f = static_cast<float>(AnchorSource::In);
  static auto constexpr out_source_f = static_cast<float>(AnchorSource::Out);
  static auto constexpr level_source_f =
      static_cast<float>(AnchorSource::Level);

public:
  static inline void migrate(Module *m) {
    for (auto step = 0; step < N; step++) {
      configure_anchors(m, step);
      disable_timed_sustain_mode(m, step);
      configure_modes(m, step);
    }
  }

private:
  // In v0, it was possible for a step to be configured GenerateMode::Sustain
  // and AdvanceMode::TimerExpires. That combination was meaningless, because
  // Sustain means "wait until triggered" and TimerExpres means "ignore all
  // triggers". So the step selector simply skipped that step, effectively
  // disabling it. We'll accomplish that by turning off the "enabled" button.
  // This creates a change of behavior in a specific condition: If the patch
  // enables the step via CV, the module will now enter the step, where the v0
  // module would skip it. I think the risk is small.
  static inline void disable_timed_sustain_mode(Module *m, int step) {
    auto const v0_advance_mode = static_cast<V0AdvanceMode>(
        m->params[v0_advance_mode_base + step].getValue());
    auto const v0_generate_mode = static_cast<V0GenerateMode>(
        m->params[v0_generate_mode_base + step].getValue());

    auto const is_timed = v0_advance_mode == V0AdvanceMode::TimerExpires;
    auto const is_sustain = v0_generate_mode == V0GenerateMode::Sustain;

    if (is_timed || is_sustain) {
      m->params[enable_step_base + step].setValue(static_cast<float>(false));
    }
  }

  static inline void configure_anchors(Module *m, int step) {
    // Copy the end level knob to the start level knob. That's needed for
    // GenerateMode::Level, and is safe for other generate modes, all of which
    // ignore the start level knob.
    m->params[start_level_base + step].setValue(
        m->params[end_level_base + step].getValue());

    auto const v0_generate_mode = static_cast<V0GenerateMode>(
        m->params[v0_generate_mode_base + step].getValue());

    switch (v0_generate_mode) {
    case V0GenerateMode::Chase:
      m->params[start_mode_base + step].setValue(sample_mode_f);
      m->params[start_source_base + step].setValue(out_source_f);
      m->params[end_mode_base + step].setValue(track_mode_f);
      m->params[end_source_base + step].setValue(in_source_f);
      break;
    case V0GenerateMode::Curve:
      m->params[start_mode_base + step].setValue(sample_mode_f);
      m->params[start_source_base + step].setValue(out_source_f);
      m->params[end_mode_base + step].setValue(track_mode_f);
      m->params[end_source_base + step].setValue(level_source_f);
      break;
    case V0GenerateMode::Hold:
      m->params[start_mode_base + step].setValue(sample_mode_f);
      m->params[start_source_base + step].setValue(out_source_f);
      m->params[end_mode_base + step].setValue(sample_mode_f);
      m->params[end_source_base + step].setValue(out_source_f);
      break;
    case V0GenerateMode::Input:
      m->params[start_mode_base + step].setValue(track_mode_f);
      m->params[start_source_base + step].setValue(in_source_f);
      m->params[end_mode_base + step].setValue(track_mode_f);
      m->params[end_source_base + step].setValue(in_source_f);
      break;
    case V0GenerateMode::Level:
      m->params[start_mode_base + step].setValue(track_mode_f);
      m->params[start_source_base + step].setValue(level_source_f);
      m->params[end_mode_base + step].setValue(track_mode_f);
      m->params[end_source_base + step].setValue(level_source_f);
      break;
    case V0GenerateMode::Sustain:
      m->params[start_mode_base + step].setValue(sample_mode_f);
      m->params[start_source_base + step].setValue(out_source_f);
      m->params[end_mode_base + step].setValue(sample_mode_f);
      m->params[end_source_base + step].setValue(out_source_f);
      break;
    default:
      break;
    }
  }

  static inline void configure_modes(Module *m, int step) {
    auto const v0_advance_mode = static_cast<V0AdvanceMode>(
        m->params[v0_advance_mode_base + step].getValue());
    auto const v0_advance_mode_ordinal = static_cast<int>(v0_advance_mode);
    auto const is_timer_expires =
        v0_advance_mode == V0AdvanceMode::TimerExpires;

    auto const v0_generate_mode = static_cast<V0GenerateMode>(
        m->params[v0_generate_mode_base + step].getValue());
    auto const is_sustain = v0_generate_mode == V0GenerateMode::Sustain;

    auto const trigger_mode_ordinal = cx::max(v0_advance_mode_ordinal - 1, 0);
    auto const trigger_mode = static_cast<TriggerMode>(trigger_mode_ordinal);

    auto is_interruptible = is_sustain || !is_timer_expires;
    auto const interrupt_mode =
        is_interruptible ? InterruptMode::Advance : InterruptMode::Ignore;

    auto completion_mode =
        is_sustain ? CompletionMode::Sustain : CompletionMode::Advance;

    m->params[trigger_mode_base + step].setValue(
        static_cast<float>(trigger_mode));
    m->params[interrupt_mode_base + step].setValue(
        static_cast<float>(interrupt_mode));
    m->params[completion_mode_base + step].setValue(
        static_cast<float>(completion_mode));
  }
};
} // namespace curve_sequencer
} // namespace dhe
