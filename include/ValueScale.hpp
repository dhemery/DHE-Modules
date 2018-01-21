#pragma once

#include <functional>
#include <utility>

namespace DHE {
    struct ValueScale {
        ValueScale(std::function<float()> min, std::function<float()> max) : min(std::move(min)), max(std::move(max)) {}

        ValueScale(const float min, const float max)
                : ValueScale([&]() { return min; }, [&]() { return max; }
        ) {}

        float scale(const float normalized) const {
            return normalized * (max() - min()) + min();
        };

        float scale(const bool state) const {
            return state ? max() : min();
        };

        float normalize(const float scaled) const {
            return (scaled - min()) / (max() - min());
        }

    private:
        std::function<float()> min;

        std::function<float()> max;
    };
}