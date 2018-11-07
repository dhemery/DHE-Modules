---
title: The Tapers Module
---
<img class="panel" src="panel.svg" alt="The Tapers Panel" />

A pair of knobs with adjustable tapers.

A
_[taper](https://en.wikipedia.org/wiki/Potentiometer#Resistance–position_relationship:_"taper")_
is a
_[transfer function](https://en.wikipedia.org/wiki/Transfer_function)_
that defines the relationship
between the position of the knob
and the device's output value.

_Tapers_ can produce
[linear, J-shaped, or S-shaped]({{ '/technical/curves/' | relative_url }})
tapers,
as well as inverted J and S tapers.

**WARNING:**
Modulating a _LEVEL_ knob beyond its normal range can generate outputs far
outside the knob's normal range.  This is especially true as the _CURVE_ knob
approaches either extreme.

## Controls
- **LEVEL:**
    Selects a position
    along the transfer function
    defined by the _TAPER_ panel.

- **CURVE:**
    The curvature
    of the transfer function
    applied to the _LEVEL_ knob.

    The center position
    produces a linear taper.
    Rotation away from the center
    increases the the curvature of the transfer function.
    Counterclockwise rotation
    inverts the shape of the curve.

    The shape switch **(S / J)**
    selects the general [shape]({{ '/technical/curves/' | relative_url }})
    of the transfer function.

- **UNI / BI:**
    The voltage range of the output signal.
    0 to 10V (UNI)
    or -5 to 5V (BI).

## Ports
- **OUT:**
    The output voltage,
    determined by applying the transfer function
    to the position of the _LEVEL_ knob.

- **CV:**
    A [control voltage]({{ '/technical/modulation/' | relative_url }}) signal
    to modulate the associated parameter. Each CV input has an attenuverter.

## Using Tapers

TBD
