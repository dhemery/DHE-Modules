---
layout: base
---

## The Modules

The DHE-Modules suite includes
[envelope generators](#envelope-generators),
[low frequency oscillators](#low-frequency-oscillators),
[utilities](#utilities).
and
[waveshapers](#waveshapers).

### Envelope Generators

[![_Stage_](modules/stage/stage.svg)](modules/stage/)
[![_Booster Stage_](modules/booster-stage/booster-stage.svg)](modules/booster-stage/)
[![_Hostage_](modules/hostage/hostage.svg)](modules/hostage/)
[![_Upstage_](modules/upstage/upstage.svg)](modules/upstage/)

**[_Stage_](modules/stage/)**
generates a single envelope stage.
Combine _Stages_ and other modules
to generate complex envelopes
with any number of stages.

**[_Booster Stage_](modules/booster-stage/)**
enhances _Stage's_ functionality,
adding CV inputs,
sigmoid curves,
buttons to inject triggers and other control signals,
and more.

**[_Hostage_](modules/hostage/)**
generates a hold or sustain envelope stage.

**[_Upstage_](modules/upstage/)**
adds special-purpose envelope controls
to
inject triggers,
suppress retriggers,
set an envelope's starting voltage,
and more.

### Low Frequency Oscillators

[![_Blossom_](modules/blossom/blossom.svg)](modules/blossom/)
[![_Xycloid_](modules/xycloid/xycloid.svg)](modules/xycloid/)

**[_Blossom_](modules/blossom/)**
traces a point
around the edge of a circle
as the radius of the circle varies.

**[_Xycloid_](modules/xycloid/)**
traces a fixed point
on the edge a circle
rolling along the inside or outside
of another circle.

Each draws a pleasing, two-dimensional shape,
generating a pair of complex, evolving, pulsing LFO signals.

### Utilities

[![_Cubic_](modules/cubic/cubic.svg)](modules/cubic/)
[![_Func_](modules/func/func.svg)](modules/func/)
[![_Func 6_](modules/func-6/func-6.svg)](modules/func-6/)
[![_Ranger_](modules/ranger/ranger.svg)](modules/ranger/)
[![_Tapers_](modules/tapers/tapers.svg)](modules/tapers/)

**[_Cubic_](modules/cubic/)**
offsets, scales, squares, and cubes its input signal.

**[_Func_](modules/func/)**
is a knob with a selectable range to offset or scale its input signal.

**[_Func 6_](modules/func-6/)**
is a chain of knobs with selectable ranges to offset and scale input signals.

**[_Ranger_](modules/ranger/)**
is a modulatable knob with an adjustable, modulatable range.

**[_Tapers_](modules/tapers/)**
is a pair of modulatable knobs with adjustable, modulatable tapers.

### Waveshapers

[![_Swave_](modules/swave/swave.svg)](modules/swave/)

**[_Swave_](modules/swave/)**
is a waveshaper with a unique sigmoid transfer function.

## See Also

- [Release Notes]({{ '/release-notes/' | relative_url }})
- [Generating Single-Stage Envelopes]({{ '/guides/generating-single-stage-envelopes/' | relative_url }})
- [Generating Multi-Stage Envelopes]({{ '/guides/generating-multi-stage-envelopes/' | relative_url }})
- [More Usage Ideas]({{ '/guides/' | relative_url }})
- [Technical Details]({{ '/technical/' | relative_url }})
