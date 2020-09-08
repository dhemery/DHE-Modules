#pragma once

#include "anchor.h"
#include "controls.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curve_sequencer {
static auto constexpr module_json_version = 1;

template <int N> class PresetV0 {
  // Params whose meanings changed from json v0 to v1. We must translate the
  // values of these params whenever we load a v0 preset.
  struct V0Param {
    enum {
      StepGenerateMode = ParamIds<N>::StepTriggerMode,  // Values: GenerateMode
      StepAdvanceMode = ParamIds<N>::StepInterruptMode, // Values: AdvanceMode
    };
  };

  enum class V0GenerateMode {
    Curve,   // Start: Sample OUT; End: Track LEVEL
    Hold,    // Start: Sample OUT; End: Sample Out; Int: Ignore; At End: Advance
    Sustain, // Start: Sample OUT; End: Sample Out; Int: Advance; At End:
             // Sustain
    Input,   // Start: Track IN; End: Track IN
    Chase,   // Start: Sample OUT; End: Track IN
    Level,   // Start: Track LEVEL; End: Track LEVEL
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

  static inline void initialize_start_level(rack::engine::Module *m, int step) {
    m->params[ParamIds<N>::StepStartLevel + step].setValue(
        m->params[ParamIds<N>::StepEndLevel + step].getValue());
  }

  static inline void migrate_generate_mode(rack::engine::Module *m, int step) {
    auto constexpr sample_mode = static_cast<float>(AnchorMode::Sample);
    auto constexpr track_mode = static_cast<float>(AnchorMode::Track);
    auto constexpr in_source = static_cast<float>(AnchorSource::In);
    auto constexpr out_source = static_cast<float>(AnchorSource::Out);
    auto constexpr level_source = static_cast<float>(AnchorSource::Level);
    auto constexpr start_mode_base =
        static_cast<int>(ParamIds<N>::StepStartAnchorMode);
    auto constexpr start_source_base =
        static_cast<int>(ParamIds<N>::StepStartAnchorSource);
    auto constexpr end_mode_base =
        static_cast<int>(ParamIds<N>::StepEndAnchorMode);
    auto constexpr end_source_base =
        static_cast<int>(ParamIds<N>::StepEndAnchorSource);

    auto const generate_mode = static_cast<V0GenerateMode>(
        m->params[V0Param::StepGenerateMode].getValue());

    switch (generate_mode) {
    case V0GenerateMode::Chase:
      m->params[start_mode_base + step].setValue(sample_mode);
      m->params[start_source_base + step].setValue(out_source);
      m->params[end_mode_base + step].setValue(track_mode);
      m->params[end_source_base + step].setValue(in_source);
      break;
    case V0GenerateMode::Curve:
      m->params[start_mode_base + step].setValue(sample_mode);
      m->params[start_source_base + step].setValue(out_source);
      m->params[end_mode_base + step].setValue(track_mode);
      m->params[end_source_base + step].setValue(level_source);
      break;
    case V0GenerateMode::Hold:
      m->params[start_mode_base + step].setValue(sample_mode);
      m->params[start_source_base + step].setValue(out_source);
      m->params[end_mode_base + step].setValue(sample_mode);
      m->params[end_source_base + step].setValue(out_source);
      break;
    case V0GenerateMode::Input:
      m->params[start_mode_base + step].setValue(track_mode);
      m->params[start_source_base + step].setValue(in_source);
      m->params[end_mode_base + step].setValue(track_mode);
      m->params[end_source_base + step].setValue(in_source);
      break;
    case V0GenerateMode::Level:
      m->params[start_mode_base + step].setValue(track_mode);
      m->params[start_source_base + step].setValue(level_source);
      m->params[end_mode_base + step].setValue(track_mode);
      m->params[end_source_base + step].setValue(level_source);
      break;
    case V0GenerateMode::Sustain:
      m->params[start_mode_base + step].setValue(sample_mode);
      m->params[start_source_base + step].setValue(out_source);
      m->params[end_mode_base + step].setValue(sample_mode);
      m->params[end_source_base + step].setValue(out_source);
      break;
    default:
      break;
    }
  }

public:
  static inline void migrate(rack::engine::Module *m) {
    for (auto step = 0; step < N; step++) {
      initialize_start_level(m, step);
      migrate_generate_mode(m, step);
    }
  }
};
} // namespace curve_sequencer
} // namespace dhe
