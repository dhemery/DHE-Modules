#pragma once

#include <gmock/gmock.h>

class MockLight {
public:
  MOCK_METHOD(void, setBrightness, (float) );
};
