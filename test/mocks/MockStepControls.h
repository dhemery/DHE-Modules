#pragma once

#include "modules/curve-sequencer/StepControls.h"

class MockStepControls : public dhe::curve_sequencer::StepControls {
public:
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(int, generateMode, (int), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(int, sustainMode, (int), (const, override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
};
