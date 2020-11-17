---
title: The Truth Modules
---
<img class="faceplate" src="truth.svg" alt="The Truth 3 Faceplate" />

Customizable truth tables with 2, 3, or 4 inputs.

_Truth 3_ is shown, and described below.
_Truth 2_ and _Truth 4_ offer similar functionality,
with 2 and 4 inputs, respectively.

## Controls

### Inputs
- **A**, **B**, and **C/GATE:**
    The input signals for the truth table.
    A voltage above 0.5V (half a volt)
    is considered _high_.
    A voltage of 0.5V or below
    is considered _low_.

    Beside each input is an "override" button
    that sets the input signal high while pressed.

### The Truth Table

#### Columns

The first three columns
represent the three inputs.

1.  Column 1 represents either the **A** input
    or the truth table's current state (**Q**).
    Use the button at the top of the column
    to switch between the two.

    See [using **Q** as an input](#using-q-as-an-input)
    for more details.

1.  Column 2 represents the **B** input.

1.  Column 3 represents the _condition_ of the **C/GATE** input.
    The button at the top of the column
    selects a condition to apply.
    When the **C/GATE** input satisfies the condition,
    the column is true.

    The conditions are:

    - _HIGH:_
        The gate is high.

    - _LOW:_
        The gate is low.

    - _RISE:_
        The gate rises.

    - _FALL:_
        The gate falls.

    - _EDGE:_
        The gate changes (rises or falls).

    See [using input **C** as gate or clock](#using-input-c-as-a-gate-or-clock)
    for more details.


1.  Column 4 (the **Q**) column
    specifies the output of the truth table
    for each possible state of the inputs.

#### Rows

-   Below the dark header row,
    the truth table has eight more rows,
    one for each possible state of the inputs.
-   In each row,
    the first three columns
    represent a possible state of the three inputs.
-   The button in the **Q** column
    selects the value (true or false)
    to output when the inputs
    match the row's state.

### Outputs

- **Q:**
    The value of the truth table
    for the given inputs.
    10V represents true,
    and 0V represents false.

- **¬Q:**
    The negation of Q.

## Usage Notes

### Using **Q** as an input

When **Q** is selected as an input,
the **A** input port and button are ignored.

Additional details… TBD

### Using input **C/GATE** as a gate or clock

Note that the _RISE_, _FALL_, and _EDGE_ conditions
are satisfied by _transitions_.
Each transition lasts for a single sample.

In contrast, _HIGH_ and _LOW_ evaluate the _state_ of the input.
The input may remain in that state indefinitely.

Additional details… TBD
