---
title: The Swave Module
---
<img class="panel" src="panel.svg" alt="The Swave Panel" />

A waveshaper with a unique sigmoid transfer function.

**Note:**
_Swave_ assumes that its input signal
is in the range [-5V,5V],
the standard range
for audio and bipolar CV signals
in VCV Rack.

See also:
- [J and S Curves]({{ '/technical/curves/' | relative_url }})
- [Normalized Tunable Sigmoid Function]({{ 'technical/sigmoid/' | relative_url }}):
    the _transfer function_
    that _Swave_ applies
    to shape its input signals

## Controls
- **CURVE:**
    The curvature of the transfer function.
    The shape switch **(S / J)**
    selects the
    [shape]({{ '/technical/curves/' | relative_url }})
    of the transfer function.

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
