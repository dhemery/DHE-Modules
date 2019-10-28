#pragma once

#include <gmock/gmock.h>

class MockParam {
public:
  MOCK_METHOD(float, getValue, ());
  MOCK_METHOD(void, setValue, (float) );
};
