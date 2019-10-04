#pragma once

#include "modules/components/Latch.h"

#include <gmock/gmock.h>

struct MockLatch : public dhe::Latch {
  MOCK_METHOD(bool, isEdge, (), (const, override));
  MOCK_METHOD(bool, isHigh, (), (const, override));
  MOCK_METHOD(void, set, (bool, bool), (override));
  MOCK_METHOD(void, step, (), (override));
};
