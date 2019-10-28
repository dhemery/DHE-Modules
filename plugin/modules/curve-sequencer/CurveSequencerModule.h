#pragma once

#include "CurveSequencer.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {

namespace curve_sequencer {

  template <int N> class CurveSequencerModule : public rack::engine::Module {
    using CurveSequencer
        = CurveSequencer<N, rack::engine::Input, rack::engine::Output, rack::engine::Param, rack::engine::Light>;

  public:
    CurveSequencerModule();
    ~CurveSequencerModule() override = default;

    void process(const ProcessArgs &args) override { sequencer.execute(args.sampleTime); }

    auto output() const -> float;
    void setOutput(float voltage);

    auto curvature(int stepIndex) const -> float;
    auto duration(int stepIndex) const -> float;
    auto level(int stepIndex) const -> float;
    auto taperSelection(int stepIndex) const -> int;

  private:
    CurveSequencer sequencer{inputs, outputs, params, lights};
  };
} // namespace curve_sequencer

} // namespace dhe
