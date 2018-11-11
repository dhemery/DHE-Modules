---
title: The Juster Module
---
<img class="panel" src="panel.svg" alt="The Juster Panel" />

A signal chain that applies a series of selectable functions.

Each input is normaled to the output of the function above it.

## Controls
- **GAIN / AV / OFFSET:**
  Selects a function to apply to the input.

  - _GAIN_
    multiplies the input
    by a value in the range
    [0, 2].

  - _AV_ (attenuverter)
    multiples the input
    by a value in the range
    [-1, 1].

  - _OFFSET_
    adds a value in the range [-5, 5]
    to the input.

- **KNOB:**
  Adjusts the input
  according to the selected function.

## Ports
- **IN:**
  The input to a function.

  Each input is normaled to the output of the function above it.

- **OUT:**
  The output signal from a function.

