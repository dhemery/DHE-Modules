#pragma once

#include "modules/curve-sequencer/StepControls.h"
#include "modules/components/Taper.h"

class MockStepControls : public dhe::curve_sequencer::StepControls {
public:
  MOCK_METHOD(float, curvature, (int), (const, override));
  MOCK_METHOD(float, duration, (int), (const, override));
  MOCK_METHOD(bool, isEnabled, (int), (const, override));
  MOCK_METHOD(int, generateMode, (int), (const, override));
  MOCK_METHOD(float, level, (int), (const, override));
  MOCK_METHOD(float, output, (), (const, override));
  MOCK_METHOD(void, setGenerating, (int, bool), (override));
  MOCK_METHOD(void, setSustaining, (int, bool), (override));
  MOCK_METHOD(void, setOutput, (float), (override));
  MOCK_METHOD(int, sustainMode, (int), (const, override));
  MOCK_METHOD(int, taperSelection, (int), (const, override));
};
