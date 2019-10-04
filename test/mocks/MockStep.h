#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Step.h"

#include <gmock/gmock.h>

struct MockStep : public dhe::curve_sequencer::Step {
  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(void, process, (dhe::Latch const &, float), (override));
};
