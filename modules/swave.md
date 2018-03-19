---
title: The Swave Module
---
<img class="panel" src="panel.svg" alt="The Swave Panel" />

A waveshaper with a unique sigmoid transfer function.

## Controls
- **CURVE:**
    The curvature of the transfer function.
    The center position (no curvature)
    produces a linear transfer function
    (no waveshaping).
    Positive curvature produces
    an S- or J-shaped transfer function.
    Negative curvature produces
    an inverted S- or J-shaped transfer function.
    At extreme positions the curvature
    of the transfer function is severe.

    The shape switch **(S / J)**
    selects the general shape
    of the transfer function:
    Either J-shaped or sigmoid (S-shaped).

    The _CURVE_ knob is modulated by the **CV** input.
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

## Ports

- **IN:**
    The input signal to be shaped
    by the transfer function.

- **OUT:**
    The shaped signal.
