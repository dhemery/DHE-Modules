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
        if (deferGate.isLow()) ramp.step();
        envelopeTrigger.step();

        outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
        outputs[EOC_TRIGGER_OUT].value = gate(eocPulse.process(rack::engineGetSampleTime()));
        outputs[ACTIVE_GATE_OUT].value = gate(deferGate.isHigh() || ramp.isRunning());
    }

    float Stage::stageIn() const { return unipolar(inputs[STAGE_IN].value); }
    // These constants yield ramp durations of:
    //    knob fully ccw  : .002417s
    //    knob dead center: 1s
    //    knob fully cw   : 10s
#define DURATION_KNOB_CURVATURE (4.0f)
#define DURATION_KNOB_MAX (0.88913970f)
#define DURATION_KNOB_MIN (1.0f - DURATION_KNOB_MAX)
#define DURATION_SCALE (16.0f)

    float Stage::duration() const {
        float knob = params[DURATION_KNOB].value;
        float squeezed = scaled(knob, DURATION_KNOB_MIN, DURATION_KNOB_MAX);
        float curved = shaped(squeezed, DURATION_KNOB_CURVATURE);
        return scaled(curved, 0.0f, DURATION_SCALE);
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

    float Stage::envelopeOut() {
        return scaled(shaped(ramp.phase(), shape()), stageInputFollower.value(), level());
    }
} // namespace DHE
