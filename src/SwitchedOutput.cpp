#include <SwitchedOutput.hpp>
#include <functional>
#include "rack.hpp"

namespace DHE {
    SwitchedOutput::SwitchedOutput(rack::Output &output, const std::function<float()> &trueValue, const std::function<float()> &falseValue) {
        this->output = &output;
        this->trueValue = trueValue;
        this->falseValue = falseValue;
    }

    void SwitchedOutput::send(bool state) {
        output->value = state ? trueValue() : falseValue();
    }

    std::unique_ptr<SwitchedOutput> SwitchedOutput::choosing(rack::Output &output, const std::function<float()> &trueValue, const std::function<float()> &falseValue) {
        return std::unique_ptr<SwitchedOutput>(new SwitchedOutput(output, trueValue, falseValue));
    }

    std::unique_ptr<SwitchedOutput> SwitchedOutput::gate(rack::Output &output) {
        return choosing(output, []() { return 10.0f; }, []() { return 0.0f; });
    }

    std::unique_ptr<SwitchedOutput> SwitchedOutput::pulse(rack::Output &output) {
        return choosing(output, []() { return 10.0f; }, []() { return 0.0f; });
    }
}