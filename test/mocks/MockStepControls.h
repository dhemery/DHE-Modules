#pragma once

#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/StepMode.h"

struct MockStepControls : public dhe::curve_sequencer::StepControls {
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(dhe::curve_sequencer::StepMode, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(dhe::curve_sequencer::StepMode, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};
