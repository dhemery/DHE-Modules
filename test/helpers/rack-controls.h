#pragma once

namespace test {
namespace fake {

struct Light {
  auto getBrightness() const -> float { return brightness_; } // NOLINT
  void setBrightness(float b) { brightness_ = b; }            // NOLINT

private:
  float brightness_{-1111.F};
};

struct Param {
  auto getValue() const -> float { return value_; } // NOLINT
  void setValue(float v) { value_ = v; }            // NOLINT

private:
  float value_{-2222.F};
};

struct Port {
  auto getVoltage() const -> float { return voltage_; } // NOLINT
  void setVoltage(float v) { voltage_ = v; }            // NOLINT

private:
  float voltage_{-3333.F};
};

} // namespace fake
} // namespace test
