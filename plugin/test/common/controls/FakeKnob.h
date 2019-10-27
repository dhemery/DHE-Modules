#pragma once

struct FakeKnob {
  auto getValue() const -> float {
    return value;
  }
  void setValue(float newValue) {
    value = newValue;
  }
  float value;
};
