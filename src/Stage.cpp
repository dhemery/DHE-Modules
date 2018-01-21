#include <cmath>
#include "Stage.hpp"

inline float gate(bool state) {
    return state ? 10.0f : 0.0f;
}

inline float unipolar(float f) {
    return rack::clampf(f, 0.0f, 10.0f);
}

inline float shaped(float phase, float shape) {
    return shape >= 0.0f ? pow(phase, shape + 1.0f) : 1.0f - shaped(1.0f - phase, -shape);
}

inline float scaled(float f, float min, float max) {
    return rack::crossf(min, max, f);
}

namespace DHE {
    void Stage::step() {
        deferGate.step();
        envelopeStep();

        outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
        outputs[EOC_TRIGGER_OUT].value = gate(eocPulse.process(rack::engineGetSampleTime()));
        outputs[ACTIVE_GATE_OUT].value = gate(deferGate.isHigh() || ramp.isRunning());
    }

    float Stage::stageIn() const { return unipolar(inputs[STAGE_IN].value); }

    float Stage::duration() const {
        return pow(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) * DURATION_SCALE;
    }

    float Stage::level() const {
        return params[LEVEL_KNOB].value;
    }

    float Stage::shape() const { return params[SHAPE_KNOB].value; }

    void Stage::defer() {
        ramp.stop();
        stageInputFollower.follow();
    }

    void Stage::resume() {
        stageInputFollower.pause();
    }

    void Stage::envelopeStart() {
        stageInputFollower.pause();
        ramp.start();
    }

    void Stage::envelopeStep() {
        if (deferGate.isLow()) ramp.step();
        envelopeTrigger.step();
    }

    float Stage::envelopeOut() {
        return scaled(shaped(ramp.phase(), shape()), stageInputFollower.value(), level());
    }

    float Stage::rampStepSize() const {
        return rack::engineGetSampleTime() / duration();
    }

} // namespace DHE
