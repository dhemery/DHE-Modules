#include <BinaryOutput.hpp>
#include <functional>
#include "rack.hpp"

namespace DHE {
    BinaryOutput::BinaryOutput(rack::Output &output, float valueWhenTrue, float valueWhenFalse)
    : output(output), valueWhenTrue(valueWhenTrue), valueWhenFalse(valueWhenFalse) {
    }

    void BinaryOutput::send(bool state) {
        output.value = state ? valueWhenTrue : valueWhenFalse;
    }

    std::unique_ptr<BinaryOutput> BinaryOutput::gate(rack::Output &output) {
        return std::unique_ptr<BinaryOutput>(new BinaryOutput(output, 10.0f, 0.0f));
    }

    std::unique_ptr<BinaryOutput> BinaryOutput::pulse(rack::Output &output) {
        return std::unique_ptr<BinaryOutput>(new BinaryOutput(output, 10.0f, 0.0f));
    }
}