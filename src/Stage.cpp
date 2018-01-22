#include <cmath>
#include "Stage.hpp"
#include "Functions.hpp"

// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_CURVATURE (4.0f)
#define DURATION_SCALE (16.0f)
#define DURATION_SQUEEZED_MAX (0.88913970f)
#define DURATION_SQUEEZED_MIN (1.0f - DURATION_SQUEEZED_MAX)
#define ENVELOPE_CURVATURE_MAX 4.0f

inline float boolToGateVoltage(bool state) {
    return state ? 10.0f : 0.0f;
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
        if (deferGate.isLow()) ramp.step();
        envelopeTrigger.step();

        outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
        outputs[EOC_TRIGGER_OUT].value = boolToGateVoltage(eocPulse.process(rack::engineGetSampleTime()));
        outputs[ACTIVE_GATE_OUT].value = boolToGateVoltage(deferGate.isHigh() || ramp.isRunning());
    }

    float Stage::stageIn() const { return inputs[STAGE_IN].value; }

    float Stage::duration() const {
        float knob = params[DURATION_KNOB].value;
        std::function<float(float)> squeezed(scalingToRange(DURATION_SQUEEZED_MIN, DURATION_SQUEEZED_MAX));
        std::function<float(float)> curved([](float f) { return pow(f, DURATION_CURVATURE); });
        std::function<float(float)> scaled(scalingToRange(0.0f, DURATION_SCALE));
        return scaled(curved(squeezed(knob)));
    }

    float Stage::level() const {
        return toUnipolarVoltage(params[LEVEL_KNOB].value);
    }

    float Stage::shape() const {
        std::function<float(float)> toShape(scalingToRange(-ENVELOPE_CURVATURE_MAX, ENVELOPE_CURVATURE_MAX));
        return toShape(params[SHAPE_KNOB].value);
    }

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

    float Stage::envelopeOut() {
        return scaled(shaped(ramp.phase(), shape()), stageInputFollower.value(), level());
    }
} // namespace DHE
