#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Step.h"

#include <gmock/gmock.h>

class MockStep : public dhe::curve_sequencer::Step {
public:
  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(Step::State, process, (dhe::Latch const &, float), (override));
};
