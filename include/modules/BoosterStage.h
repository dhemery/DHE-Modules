#pragma once
#include "envelopes/StageStateMachine.h"
#include "modules/components/Taper.h"
#include "modules/controls/CommonInputs.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/DurationInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class BoosterStage : public rack::engine::Module {
public:
  BoosterStage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    ActiveButton,
    CurveKnob,
    DeferButton,
    DurationKnob,
    DurationRangeSwitch,
    EocButton,
    LevelKnob,
    LevelRangeSwitch,
    ShapeSwitch,
    TriggerButton,
    ParameterCount
  };

  enum InputIds { CurveCvInput, DeferInput, DurationCvInput, LevelCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };

private:
  auto activeButton() const -> bool { return buttonIsPressed(this, ActiveButton); }

  auto deferIsHigh() const -> bool { return inputIsHigh(this, DeferInput) || buttonIsPressed(this, DeferButton); }

  auto duration() const -> float { return dhe::duration(this, DurationKnob, DurationCvInput, DurationRangeSwitch); }

  auto envelopeIn() const -> float { return inputVoltage(this, EnvelopeInput); }

  auto eocButton() const -> bool { return buttonIsPressed(this, EocButton); }

  void forward() { sendOut(envelopeIn()); }

  void generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

  auto level() const -> float {
    return scaledRotation<2>(this, LevelKnob, LevelCvInput, LevelRangeSwitch, signalRanges);
  }

  void sendActive() {
    auto const voltage = unipolarSignalRange.scale(isActive || activeButton());
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc() {
    auto const voltage = unipolarSignalRange.scale(isEoc || eocButton());
    outputs[EocOutput].setVoltage(voltage);
  }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void setActive(bool active) { isActive = active; }

  void setEoc(bool eoc) { isEoc = eoc; }

  auto taper(float input) const -> float {
    auto const curvature = dhe::curvature(this, CurveKnob, CurveCvInput);
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature);
  }

  auto triggerIsHigh() const -> bool { return inputIsHigh(this, TriggerInput) || buttonIsPressed(this, TriggerButton); }

  void finishGenerating();
  void processEoc(float sampleTime);
  bool processDefer();
  void processTrigger();
  void processGenerator(float sampleTime);
  void trackInput();
  void beginDeferring();
  void stopDeferring();
  void startGenerating();

  bool deferWasHigh{false};
  float eocPhase{1.F};
  bool isActive{false};
  bool isEoc{false};
  float stagePhase{1.F};
  float startVoltage{0.F};
  bool triggerWasHigh{false};
};
} // namespace dhe
