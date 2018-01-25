#include <cmath>
#include "Stage.hpp"

// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_CURVATURE (4.0f)
#define DURATION_SCALE (16.0f)
#define DURATION_SQUEEZED_MAX (0.88913970f)
#define DURATION_SQUEEZED_MIN (1.0f - DURATION_SQUEEZED_MAX)
#define ENVELOPE_CURVATURE_MAX 4.0f

namespace DHE {

Stage::Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
                 deferGate{[this]() { return inputs[DEFER_GATE_IN].value; }},
                 endOfCyclePulse{1e-3, &rack::engineGetSampleTime},
                 envelopeRamp{[this]() { return duration(); }, &rack::engineGetSampleTime},
                 envelopeTrigger{[this]() { return inputs[TRIGGER_IN].value; }},
                 stageInputFollower{[this]() { return stageIn(); }} {
  deferGate.onRisingEdge([this]() { defer(); });
  deferGate.onFallingEdge([this]() { resume(); });

  envelopeTrigger.onRisingEdge([this]() { startEnvelope(); });

  envelopeRamp.onEndOfCycle([this]() { endOfCyclePulse.start(); });
}

void Stage::step() {
  deferGate.step();
  envelopeRamp.step();
  envelopeTrigger.step();
  endOfCyclePulse.step();

  outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
  outputs[EOC_TRIGGER_OUT].value = toUnipolarVoltage(endOfCyclePulse.isActive());
  outputs[ACTIVE_GATE_OUT].value = toUnipolarVoltage(deferGate.isHigh() || envelopeRamp.isActive());
}

void Stage::defer() {
  envelopeTrigger.suspendFiring();
  envelopeRamp.stop();
  stageInputFollower.follow();
}

void Stage::resume() {
  stageInputFollower.pause();
  envelopeTrigger.resumeFiring();
}

void Stage::startEnvelope() {
  stageInputFollower.pause();
  envelopeRamp.start();
}

float Stage::duration() const {
  float squeezed = scaleToRange(params[DURATION_KNOB].value, DURATION_SQUEEZED_MIN, DURATION_SQUEEZED_MAX);
  float curved = pow(squeezed, DURATION_CURVATURE);
  return scaleToRange(curved, 0.0f, DURATION_SCALE);
}

float Stage::envelopeOut() const {
  float level = toUnipolarVoltage(params[LEVEL_KNOB].value);
  float curvature = scaleToRange(params[SHAPE_KNOB].value, -ENVELOPE_CURVATURE_MAX, ENVELOPE_CURVATURE_MAX);
  float curved = curve(envelopeRamp.phase(), curvature);
  return scaleToRange(curved, stageInputFollower.value(), level);
}

float Stage::stageIn() const { return inputs[STAGE_IN].value; }
} // namespace DHE
