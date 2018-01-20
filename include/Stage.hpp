#pragma once

#include "FlipFlop.hpp"
#include "Ramp.hpp"
#include "dsp/digital.hpp"
#include "rack.hpp"
#include "Freezer.hpp"

// TODO: Switch for slow, medium, and fast ramp
// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_KNOB_CURVATURE (4.0f)
#define DURATION_KNOB_MAX (0.88913970f)
#define DURATION_KNOB_MIN (1.0f - DURATION_KNOB_MAX)
#define DURATION_SCALE (16.0f)

#define SHAPE_KNOB_MAX 4.0f

namespace DHE {

    struct Stage : rack::Module {
        enum ParamIds {
            DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB, NUM_PARAMS
        };
        enum InputIds {
            ENVELOPE_IN, TRIGGER_IN, DEFER_GATE_IN, NUM_INPUTS
        };
        enum OutputIds {
            STAGE_OUT, EOC_TRIGGER_OUT, ACTIVE_GATE_OUT, NUM_OUTPUTS
        };
        enum LightIds {
            NUM_LIGHTS
        };

        Stage();

        void step() override;

    private:
        Freezer stageIn;
        std::unique_ptr<FlipFlop> envelopeTrigger;
        std::unique_ptr<FlipFlop> deferGate;

        float duration();
        float level();

        void defer();
        void resume();

        Ramp ramp;
        float envelopeStartVoltage;
        float rampStepSize();
        void startEnvelope();
        void advanceEnvelope();
        rack::PulseGenerator eocPulse;
        float envelopeOut();

    };
} // namespace DHE
