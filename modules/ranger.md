---
title: The Ranger Module
---
<img class="panel" src="panel.svg" alt="The Ranger Panel" />

A knob with an adjustable, modulatable range.

## Controls
- **LEVEL:**
    Interpolates between the values
    set by the _LIMIT_ panels.

- **LIMIT:**
    One end of the _LEVEL_ knob's range.  The middle panel sets the the value
    of the _LEVEL_ knob's fully clockwise position.  The lower panel sets the
    the value of the _LEVEL_ knob's fully counterclockwise position.

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
    A [control voltage]({{ '/technical/modulation/' | relative_url }}) signal
    to modulate the associated parameter. Each CV input has an attenuverter.

## Using Ranger

TBD
