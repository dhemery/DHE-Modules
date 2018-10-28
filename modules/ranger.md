---
title: The Ranger Module (coming soon)
---
<img class="panel" src="panel.svg" alt="The Ranger Panel" />

A knob with independently adjustable upper and lower limits.

## Controls
- **LEVEL:**
    A value in the range
    set by the _LIMIT_ panels.

- **LIMIT:**
    One end of the _LEVEL_ knob's range.
    The middle panel sets the knob's upper limit.
    The lower panel sets the lower limit.

- **UNI / BI:**
    The voltage range of the associated _LIMIT_ knob.
    0 to 10V (UNI)
    or -5 to 5V (BI).

## Ports
- **OUT:**
    The output voltage,
    selected by the _LEVEL_ knob
    within the range specified by the _LIMIT_ knobs.

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

## Using Ranger

TBD
