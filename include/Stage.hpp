#pragma once

#include "FlipFlop.hpp"
#include "Ramp.hpp"
#include "dsp/digital.hpp"
#include "rack.hpp"
#include "Follower.hpp"

#include <memory>

// TODO: Switch for slow, medium, and fast ramp


namespace DHE {

    /**
     *
     */
    struct Stage : rack::Module {
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

        Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

        void step() override;

    private:
        Ramp ramp = Ramp([this]() { return rack::engineGetSampleTime() / duration(); },
                         [this]() { eocPulse.trigger(1e-3); });
        Follower stageInputFollower = Follower([this]() { return stageIn(); });
        FlipFlop deferGate = FlipFlop(
                [this]() { return inputs[DEFER_GATE_IN].value; },
                [this]() { defer(); },
                [this]() { resume(); });
        FlipFlop envelopeTrigger = FlipFlop(
                [this]() { return inputs[TRIGGER_IN].value; },
                [this]() { if (deferGate.isLow()) envelopeStart(); });

        float duration() const;

        float level() const;

        float shape() const;

        void defer();

        void resume();

        float stageIn() const;

        // TODO: Create pulse generator that calls a consumer when it rises or falls
        rack::PulseGenerator eocPulse;

        // TODO: Move the following envelope stuff to Envelope class

        float envelopeOut();

        void envelopeStart();
    };
} // namespace DHE
