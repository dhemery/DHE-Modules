#pragma once

namespace test {
namespace fake {

  struct Light {
    auto getBrightness() const -> float { return brightness; }
    void setBrightness(float b) { brightness = b; }

  private:
    float brightness{-1111.F};
  };

  struct Param {
    auto getValue() const -> float { return value; }
    void setValue(float v) { value = v; }

  private:
    float value{-2222.F};
  };

  struct Port {
    auto getVoltage() const -> float { return voltage; }
    void setVoltage(float v) { voltage = v; }

  private:
    float voltage{-3333.F};
  };

} // namespace fake
} // namespace test
