#pragma once

#include "components/taper.h"
#include "modules/stage/mode.h"

#include <functional>

namespace test {
namespace stage {
using dhe::stage::Mode;
using dhe::taper::Taper;

struct FakeControls {
  std::function<float()> curvature{
      []() -> float { throw "FakeControls.curvature()"; }};
  std::function<bool()> defer{[]() -> float { throw "FakeControls.defer()"; }};
  std::function<float()> duration{
      []() -> float { throw "FakeControls.duration()"; }};
  std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
  std::function<bool()> gate{[]() -> float { throw "FakeControls.gate()"; }};
  std::function<float()> level{[]() -> float { throw "FakeControls.level()"; }};
  std::function<Mode()> mode{[]() -> Mode { throw "FakeControls.mode()"; }};
  std::function<void(float)> output{
      [](float v) { throw "FakeControls.output(voltage)"; }};
  std::function<void(bool)> show_active{
      [](bool a) { throw "FakeControls.show_active(active)"; }};
  std::function<void(bool)> show_eoc{
      [](bool a) { throw "FakeControls.show_eoc(eoc)"; }};
  std::function<Taper const *()> taper{
      []() -> Taper const * { throw "FakeControls.taper()"; }};
};
} // namespace stage
} // namespace test
