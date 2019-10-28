#pragma once

#include <gmock/gmock.h>

class MockPort {
public:
  MOCK_METHOD(float, getVoltage, ());
  MOCK_METHOD(void, setVoltage, (float));
};
