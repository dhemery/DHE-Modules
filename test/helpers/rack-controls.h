#pragma once

namespace test {
namespace fake {

struct Light {
  constexpr Light() = default;
  constexpr explicit Light(float brightness) : brightness_{brightness} {}
  auto getBrightness() const -> float { return brightness_; } // NOLINT
  void setBrightness(float b) { brightness_ = b; }            // NOLINT

private:
  float brightness_{-1111.F};
};

struct Param {
  constexpr Param() = default;
  constexpr explicit Param(float value) : value_{value} {}
  auto getValue() const -> float { return value_; } // NOLINT
  void setValue(float v) { value_ = v; }            // NOLINT

private:
  float value_{-2222.F};
};

struct Port {
  constexpr Port() = default;
  constexpr explicit Port(float voltage) : voltage_{voltage} {}
  auto getVoltage() const -> float { return voltage_; } // NOLINT
  void setVoltage(float v) { voltage_ = v; }            // NOLINT

private:
  float voltage_{-3333.F};
};

} // namespace fake
} // namespace test
