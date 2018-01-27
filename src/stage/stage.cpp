#include <cmath>
#include "stage.h"

namespace DHE {

Stage::Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
                 deferGate{[this] { return inputs[DEFER_GATE_IN].value; }},
                 endOfCyclePulse{1e-3, &rack::engineGetSampleTime},
                 envelopeRamp{[this] { return duration(); }, &rack::engineGetSampleTime},
                 envelopeTrigger{[this] { return inputs[TRIGGER_IN].value; }},
                 stageInputFollower{[this] { return stageIn(); }} {
  deferGate.onRisingEdge([this] { defer(); });
  deferGate.onFallingEdge([this] { resume(); });

  envelopeTrigger.onRisingEdge([this] { startEnvelope(); });

  envelopeRamp.onEndOfCycle([this] { endOfCyclePulse.start(); });
}

void Stage::step() {
  deferGate.step();
  envelopeRamp.step();
  envelopeTrigger.step();
  endOfCyclePulse.step();

  outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
  outputs[EOC_TRIGGER_OUT].value = UNIPOLAR_VOLTAGE.scale(endOfCyclePulse.isActive());
  outputs[ACTIVE_GATE_OUT].value = UNIPOLAR_VOLTAGE.scale(deferGate.isHigh() || envelopeRamp.isActive());
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
  static const Range range{1e-3, 10.0f};
  static constexpr float curvature{0.8f}; // Gives ~1s at center position

  return range.scale(sigmoid(params[DURATION_KNOB].value, curvature));
}

float Stage::shape() const {
  static constexpr float shapeKnobCurvature = -0.65f;

  return sigmoid(BIPOLAR_NORMAL.scale(params[SHAPE_KNOB].value, -1.0f, 1.0f), shapeKnobCurvature);
}

float Stage::envelopeOut() const {
  float shaped = sigmoid(envelopeRamp.phase(), shape());
  return Range::scale(shaped, stageInputFollower.value(), level());
}

float Stage::level() const { return UNIPOLAR_VOLTAGE.scale(params[LEVEL_KNOB].value); }

float Stage::stageIn() const { return inputs[STAGE_IN].value; }
} // namespace DHE
