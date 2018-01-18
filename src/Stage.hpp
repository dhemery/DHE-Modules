#pragma once
#include "Latch.hpp"
#include "Ramp.hpp"
#include "dsp/digital.hpp"
#include "rack.hpp"

// TODO: Switch for slow, medium, and fast ramp
// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_KNOB_CURVATURE (4.0)
#define DURATION_KNOB_MAX (0.88913970)
#define DURATION_KNOB_MIN (1.0 - DURATION_KNOB_MAX)
#define DURATION_SCALE (16.0)

#define SHAPE_KNOB_MAX 4.0

#define EOC_PULSE_LENGTH (1e-3)

namespace DHE {

struct Stage : rack::Module {
    enum ParamIds { DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB, NUM_PARAMS };
    enum InputIds { ENVELOPE_IN, TRIGGER_IN, DEFER_GATE_IN, NUM_INPUTS };
    enum OutputIds { STAGE_OUT, EOC_TRIGGER_OUT, ACTIVE_GATE_OUT, NUM_OUTPUTS };
    enum LightIds { NUM_LIGHTS };

    Stage();
    void step() override;

  private:
    Ramp ramp;
    Latch trigger;
    Latch deferLatch;
    rack::PulseGenerator eocPulse;
    float holdVoltage;

    void hold(float holdVoltage);

    void advanceEnvelopeIfRunning();
    void advanceEnvelope();
    void startEnvelopeIfTriggered(float startVoltage);

    float envelopeVoltage();
    float activeGateOutVoltage();
    float eocTriggerOutVoltage();
    float stageOutVoltage(float deferredVoltage);
};
} // namespace DHE
