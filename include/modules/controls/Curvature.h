#pragma once

#include <functional>

#include <engine/Module.hpp>

#include "util/sigmoid.h"

namespace dhe {
namespace curvature {

auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float(float)>;

auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId,
                         int avId, int switchId) -> std::function<float(float)>;

auto withFixedShape(rack::engine::Module *module, int knobId,
                    sigmoid::Shape const *shape) -> std::function<float(float)>;

auto rotationToTaper() -> std::function<float(float)>;
auto taperToRotation() -> std::function<float(float)>;

auto shape(rack::engine::Module *module, int switchId)
    -> std::function<sigmoid::Shape const *()>;
} // namespace curvature

} // namespace dhe
