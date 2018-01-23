#pragma once

#include <memory>

#include "dsp/digital.hpp"
#include "rack.hpp"
#include "util/DFlipFlop.hpp"
#include "util/Ramp.hpp"
#include "util/Follower.hpp"

// TODO: Switch for slow, medium, and fast ramp


namespace DHE {
struct Stage : rack::Module {
    static const char *SLUG;
    static const char *NAME;

    enum ParamIds {
        DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB, NUM_PARAMS
    };
    enum InputIds {
        STAGE_IN, TRIGGER_IN, DEFER_GATE_IN, NUM_INPUTS
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
    DFlipFlop deferGate;
    Ramp endOfCyclePulse;
    Ramp envelopeRamp;
    DFlipFlop envelopeTrigger;
    Follower stageInputFollower;

    float level() const;

    float shape() const;

    void defer();

    void resume();

    float stageIn() const;


    // TODO: Move the following envelope stuff to Envelope class

    float envelopeOut();

    void startEnvelope();

    float rate() const;
};
} // namespace DHE
