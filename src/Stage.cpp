#include "Stage.hpp"

static float gate(bool state) {
    return state ? 10.0f : 0.0f;
}

static float unipolar(float f) {
    return rack::clampf(f, 0.0f, 10.0f);
}

static float shaped(float phase, float shape) {
    return shape < 0.0f ? 1.0f - powf(1.0f - phase, 1.0f - shape) : powf(phase, shape + 1.0f);
}

static float scaled(float f, float min, float max) {
    return f * (max - min) + min;
}

namespace DHE {
    Stage::Stage()
            : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        deferGate = FlipFlop::latch(
                [this]() { return inputs[DEFER_GATE_IN].value; },
                [this]() { defer(); },
                [this]() { resume(); });
        envelopeTrigger = FlipFlop::trigger(
                [this]() { return inputs[TRIGGER_IN].value; },
                [this]() { if (deferGate->isLow()) envelopeStart(); });
    }

    void Stage::step() {
        deferGate->step();
        envelopeStep();

        outputs[STAGE_OUT].value = deferGate->isHigh() ? stageIn.value() : envelopeOut();
        outputs[EOC_TRIGGER_OUT].value = gate(eocPulse.process(rack::engineGetSampleTime()));
        outputs[ACTIVE_GATE_OUT].value = gate(deferGate->isHigh() || ramp.isRunning());
    }

    float Stage::envelopeIn() const { return unipolar(inputs[ENVELOPE_IN].value); }

    float Stage::duration() const {
        return powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
               DURATION_SCALE;
    }

    float Stage::level() const {
        return params[LEVEL_KNOB].value;
    }

    float Stage::shape() const { return params[SHAPE_KNOB].value; }

    void Stage::defer() {
        ramp.stop();
        stageIn.follow();
    }


    void Stage::resume() {
        stageIn.freeze();
    }

    void Stage::envelopeStart() {
        stageIn.freeze();
        ramp.start();
    }

    void Stage::envelopeStep() {
        if (deferGate->isLow()) ramp.step();
        envelopeTrigger->step();
    }
    float Stage::envelopeOut() {
        return scaled(shaped(ramp.phase(), shape()), stageIn.value(), level());
    }

    float Stage::rampStepSize() const {
        return rack::engineGetSampleTime() / duration();
    }

} // namespace DHE
