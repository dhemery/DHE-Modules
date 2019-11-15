#pragma once

#include "components/Latch.h"
#include "components/Taper.h"
#include "curve-sequencer/SequenceMode.h"
#include "curve-sequencer/StageMode.h"

#include <gmock/gmock.h>

class MockControls {
public:
  MOCK_METHOD(float, curvature, (int), (const));
  MOCK_METHOD(float, duration, (int), (const));
  MOCK_METHOD(dhe::curve_sequencer::StageMode, generateMode, (int), (const));
  MOCK_METHOD(float, input, (), (const));
  MOCK_METHOD(bool, isEnabled, (int), (const));
  MOCK_METHOD(bool, isGated, (), (const));
  MOCK_METHOD(bool, isLooping, (), (const));
  MOCK_METHOD(bool, isReset, (), (const));
  MOCK_METHOD(bool, isRunning, (), (const));
  MOCK_METHOD(float, level, (int), (const));
  MOCK_METHOD(void, output, (float) );
  MOCK_METHOD(float, output, (), (const));
  MOCK_METHOD(void, showGenerating, (int, bool) );
  MOCK_METHOD(dhe::taper::VariableTaper const *, taper, (int), (const));
};

class MockStepSelector {
public:
  MOCK_METHOD(int, first, (dhe::Latch const &), (const));
  MOCK_METHOD(int, successor, (int, dhe::Latch const &, bool), (const));
};

class MockGenerateMode {
public:
  MOCK_METHOD(void, enter, (int) );
  MOCK_METHOD(dhe::curve_sequencer::SequenceMode, execute, (dhe::Latch const &, float) );
  MOCK_METHOD(void, exit, ());
};

class MockSustainMode {
public:
  MOCK_METHOD(void, enter, (int) );
  MOCK_METHOD(dhe::curve_sequencer::SequenceMode, execute, (dhe::Latch const &) );
  MOCK_METHOD(void, exit, ());
};
