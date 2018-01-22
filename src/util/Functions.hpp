#pragma once

#include <functional>

namespace DHE {
    inline float
    scaleToRange(float normalized, float min, float max) {
        return normalized * (max - min) + min;
    }

    inline std::function<float(float)>
    scalingToRange(const std::function<float()> &min, const std::function<float()> &max) {
        return [&](float normalized) { return scaleToRange(normalized, min(), max()); };
    }

    inline std::function<float(float)>
    scalingToRange(float min, float max) {
        return [=](float normalized) { return scaleToRange(normalized, min, max); };
    }

    inline float
    toUnipolarVoltage(float normalized) {
        return scaleToRange(normalized, 0.0f, 10.0f);
    }
}