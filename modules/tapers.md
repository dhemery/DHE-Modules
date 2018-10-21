---
title: The Tapers Module (coming soon)
---
<img class="panel" src="panel.svg" alt="The Tapers Panel" />

A pair of knobs with adjustable tapers.

## Controls
- **LEVEL:**
    The input
    to the transfer function
    defined by the _TAPER_ panel.

- **TAPER:**
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

## Ports
- **OUT:**
    The output voltage,
    determined by applying the transfer function
    to the position of the _LEVEL_ knob.

    The polarity switch **(UNI / BI)**
    selects the range of the output voltage:
    Either 0 to 10V (UNI)
    or -5 to 5V (BI).

- **CV:**
    Control voltage signal that modulates the associated knob.
    The modulation is calibrated so that:
    - A 0V _CV_ signal yields the value of the knob.
    - A 5V _CV_ signal yields the value
        as if the knob were rotated 50%
        clockwise
        of its actual position.
    - A -5V _CV_ signal yields the value
        as if the knob were rotated 50%
        counterclockwise
        of its actual position.

    Each _CV_ port
        is accompanied by a small *attenuverter* (**- +**) knob,
        which adjusts the strength and direction
        of the the _CV_ signal's effect
        on the modulated knob.

## Using Tapers
