#include "Freezer.hpp"

namespace DHE {
    Freezer::Freezer(const std::function<float()> &supplier) {
        freshValue = supplier;
        freeze();
    }

    float Freezer::value() {
        return frozen ? frozenValue : freshValue();
    }

    void Freezer::follow() {
        frozen = false;
    }

    void Freezer::freeze() {
        frozenValue = freshValue();
        frozen = true;
    }
}
