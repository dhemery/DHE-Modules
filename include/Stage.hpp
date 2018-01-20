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
        Ramp ramp = Ramp([this]() { return rampStepSize(); },
                         [this]() { eocPulse.trigger(1e-3); });

        Freezer stageIn = Freezer([this]() { return envelopeIn(); });

        std::unique_ptr<FlipFlop> envelopeTrigger;
        std::unique_ptr<FlipFlop> deferGate;

        float duration() const;

        float level() const;

        float shape() const;

        void defer();

        void resume();

        float envelopeIn() const;


        // TODO: Create pulse generator that calls a consumer when it rises or falls
        rack::PulseGenerator eocPulse;

        // TODO: Move the following envelope stuff to Envelope class
        float rampStepSize() const;

        float envelopeOut();

        void envelopeStart();

        void envelopeStep();
    };
} // namespace DHE
