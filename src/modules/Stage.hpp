#pragma once

#include "dsp/digital.hpp"
#include "rack.hpp"
#include "FlipFlop.hpp"
#include "Ramp.hpp"
#include "Follower.hpp"

#include <memory>

// TODO: Switch for slow, medium, and fast ramp


namespace DHE {
    struct Stage : rack::Module {

        static const std::string SLUG;
        static const std::string NAME;

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
        Ramp ramp;
        Follower stageInputFollower;
        FlipFlop deferGate;
        FlipFlop envelopeTrigger;

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

        void startEnvelope();
    };
} // namespace DHE
