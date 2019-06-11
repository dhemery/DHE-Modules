---
title: The Ranger Module
---
<img class="faceplate" src="ranger.svg" alt="The Ranger Faceplate" />

A modulatable knob with an adjustable, modulatable range.

## Controls
- **LEVEL:**
    Interpolates between the values
    set by the _CCW_ and _CW_ panels.

- **CCW:**
    Sets the counterclockwise (CCW) end of the _LEVEL_ knob's range.

- **CW:**
    Sets the clockwise (CW) end of the _LEVEL_ knob's range.

- **UNI / BI:**
    The voltage range of the associated knob.
    0 to 10V (UNI)
    or -5 to 5V (BI).

## Ports
- **OUT:**
    The output voltage,
    selected by the _LEVEL_ knob
    within the range specified by the _CCW_ and _CW_ knobs.

- **CV:**
    A [control voltage]({{ '/technical/modulation/' | relative_url }}) signal
    to modulate the associated parameter. Each CV input has an attenuverter.
    
    Note that control voltage can drive any of Ranger's knobs beyond its normal limit.

## Using Ranger

TBD
