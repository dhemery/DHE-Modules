#pragma once

#include <functional>

namespace DHE {
    inline float clamp(float in, float low, float high) {
        if (in < low) return low;
        if (in > high) return high;
        return in;
    }

    inline float scaleToRange(float normalized, float min, float max) {
        return normalized * (max - min) + min;
    }

    inline float toUnipolarVoltage(float normalized) {
        return scaleToRange(normalized, 0.0f, 10.0f);
    }

    inline float toUnipolarVoltage(bool state) {
        return state ? 10.0f : 0.0f;
    }

    inline std::function<float(float)>
    scalingToRange(const std::function<float()> &min, const std::function<float()> &max) {
        return [&](float normalized) { return scaleToRange(normalized, min(), max()); };
    }

    inline std::function<float(float)> scalingToRange(float min, float max) {
        return [=](float normalized) { return scaleToRange(normalized, min, max); };
    }
}