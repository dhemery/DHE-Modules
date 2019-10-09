#pragma once

#include "modules/curve-sequencer/SequenceControls.h"

#include <gmock/gmock.h>

class MockSequenceControls : public dhe::curve_sequencer::SequenceControls {
public:
  MOCK_METHOD(bool, gate, (), (const, override));
  MOCK_METHOD(bool, isRunning, (), (const, override));
  MOCK_METHOD(int, selectionLength, (), (const, override));
  MOCK_METHOD(int, selectionStart, (), (const, override));
};
