---
title: The Swave Module
---
<img class="faceplate" src="swave.svg" alt="The Swave Faceplate" />

A waveshaper with a unique sigmoid transfer function.

**Note:**
_Swave_ does not apply anti-aliasing.

See also:
-   [J and S Curves](/technical/curves/).
-   [Normalized Tunable Sigmoid Function](/technical/sigmoid/):
    the _transfer function_
    that _Swave_ applies
    to shape its input signals.

## Controls
-   **CURVE:**
    The curvature of the transfer function.
    The shape switch **(S / J)**
    selects the
    [shape](/technical/curves/)
    of the transfer function.

-   The range switch **(UNI / BI)**
    selects the signal range on which _Swave_ operates.

    _Swave_ clamps the input signal to the selected range
    before shaping it.

## Ports

-   **IN:**
    The input signal to be shaped
    by the transfer function.

-   **OUT:**
    The shaped signal.

-   **CV:**
    A [control voltage](/technical/modulation/) signal
    to modulate the _CURVE_ parameter.
