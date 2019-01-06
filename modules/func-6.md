---
title: The Func 6 Module
---
<img class="faceplate" src="func-6.svg" alt="The Func 6 Faceplate" />

A chain of knobs with selectable ranges to offset and scale input signals.

Each row of controls is a channel that applies a selected function to its input signal.

Each channel's input is normaled to the output of the channel above it.

## Controls
Each channel includes the following controls.

- **ADD / MULT:**
    Selects the function to apply to the input signal.

    - _ADD_ offsets the signal by adding the selected amount to it.
    - _MULT_ scales the signal by multiplying it by the selected amount.

    This switch also selects
    an appropriate set of ranges
    for the function.
    See the _RANGE_ button, below.

- **KNOB:**
    Selects the amount
    to offset or scale
    the input signal.

- **RANGE (button):**
    Selects the range of the knob.

    The ranges depend on the function selected by the _ADD / MULT_ switch.
    For each function,
    this button cycles through four ranges.

    - _ADD_ ranges:
        - 0–5
        - ±5
        - 0–10
        - ±10
    - _MULT_ ranges:
        - 0–1 (attenuator)
        - ±1 (attenuverter)
        - 0–2 (gain)
        - ±2

## Ports
Each channel includes the following ports.

- **IN:**
    The input to the function.

    Each _IN_ port is normaled to the _OUT_ port of the channel above it.
    That is,
    if a channel's _IN_ port is not connected to a wire,
    the channel takes its input
    from the _OUT_ port
    of the channel above.

- **OUT:**
    The output signal from the function.
