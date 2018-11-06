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

## Ports

- **IN:**
    The input signal to be shaped
    by the transfer function.

- **OUT:**
    The shaped signal.

- **CV:**
    A [control voltage]({{ '/technical/modulation/' | relative_url }}) signal
    to modulate the _CURVE_ parameter.

