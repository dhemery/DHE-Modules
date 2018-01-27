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
  static constexpr float minDuration{1e-3f};
  static constexpr float maxDuration{10.0f};
  static constexpr float durationCurvature{0.8f}; // Gives ~1s at center position

  return scaleToRange(sigmoid(params[DURATION_KNOB].value, durationCurvature), minDuration, maxDuration);
}

float Stage::shape() const {
  static constexpr float shapeKnobCurvature = -0.65f;

  return sigmoid(scaleToRange(params[SHAPE_KNOB].value, -1.0f, 1.0f), shapeKnobCurvature);
}

float Stage::envelopeOut() const {
  float shaped = sigmoid(envelopeRamp.phase(), shape());
  return scaleToRange(shaped, stageInputFollower.value(), level());
}

float Stage::level() const { return toUnipolarVoltage(params[LEVEL_KNOB].value); }

float Stage::stageIn() const { return inputs[STAGE_IN].value; }
} // namespace DHE
