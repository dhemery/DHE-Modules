---
title: Modulation
---

Many modules' parameter knobs can be modulated by control voltage (CV) signals.
CV input ports may be accompanied by [attenuverters](#attenuverters).

# Calibration

In DHE-Modules, modulation by CV is calibrated to expect a CV signal in the
range [-5V,5V].
- A 0V CV signal yields the current value of the knob.
- A 5V CV signal yields the value as if the knob were rotated 50% clockwise of
  its actual position.
- A -5V CV signal yields the value as if the knob were rotated 50%
  counterclockwise of its actual position.


**WARNING:**
Modulation can yield values far outside a knob's normal range.

For example:
- With a knob in its center position, CV signals greater than 5V produce values
  higher than the knob's normal range.
- With a knob fully clockwise, positive CV signals produce values higher than
  the knob's normal range.

# Attenuverters

A CV input port may be accompanied by a small *attenuverter* (**- +**) knob.
This knob can attenuate and invert the effect of the CV signal on the modulated
knob.
- Full clockwise rotation results in the CV signal having its full effect on
  the knob.
- Values between the full clockwise position and the center position attenuate
  the CV signal, lessening its effect.
- The center position fully attenuates the CV signal so that it has no effect.
- Fully counterclockwise inverts the CV signal, so that a positive voltage
  reduces the value of the knob and a negative voltage increases the value of
  the knob.
- Values between the full counterclockwise position and the center position
  both invert and attenuate the effect of the CV signal.

