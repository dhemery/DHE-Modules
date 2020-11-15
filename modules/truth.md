---
title: The Truth Modules
---
<img class="faceplate" src="truth.svg" alt="The Truth 3 Faceplate" />

Applies a custom-designed truth table
to a set of inputs.

_Truth 3_ is shown.
_Truth 4_ offers similar functionality,
but for 4 inputs.

## Inputs
- **A**, **B**, and **C:**
    The input signals for the truth table.
    A voltage above 0.5V (half a volt)
    is considered _high_.
    A voltage of 0.5V or below
    is considered _low_.

    Each input signal is evaluated by a _condition_
    (see below)
    to derive its true/false value for the truth table.

    Beside each input is an "override" button
    that sets the input signal high while pressed.

## The Truth Table

-   **Condition Button**
    (below each input column label):
    The selected condition
    determines the truth value
    of the input signal.
    An input is _true_
    if its input signal satisfies the selected condition.
    The conditions are:

    - _HIGH:_
        The input signal is high.

    - _LOW:_
        The input signal is low.

    - _RISE:_
        The input signal rises.

    - _FALL:_
        The input signal falls.

    - _EDGE:_
        The input signal changes (rises or falls).

    Note that the _RISE_, _FALL_, and _EDGE_ conditions
    are satisfied by _events_;
    that is, _changes_ in the input signal.
    Each event lasts for a single sample.

    In contrast, _HIGH_ and _LOW_ evaluate the _state_ of the input.

-   **Outcome Button**
    (in each row of the Q column):
    Determines the value of the truth table
    when the input conditions match the row.
    The outcomes are:

    - _T:_
        The **Q** output port emits 10V (true)
        and **¬Q** emits 0V.

    - _F:_
        The **Q** output port emits 0V (false)
        and **¬Q** emits 10V.

## Outputs

- **Q:**
    The value of the truth table
    for the given inputs
    and conditions.
    10V represents true,
    and 0V represents false.

- **¬Q:**
    The negation of Q.
