---
title: The Fuzzy Logic Z Module
---
<img class="faceplate" src="fuzzy-logic-z.svg" alt="The Fuzzy Logic Z Faceplate" />

_Fuzzy Logic Z_ combines signals by applying fuzzy logic operators
based on Lofti Zadeh's definitions.

_Fuzzy Logic Z_ is arranged in two independent columns
with identical functionality.
Only the left column, which operates on the A and B inputs, is described below.
The right column applies the same operators to the C and D inputs.

**WARNING:**
_Fuzzy Logic Z_
can emit **very large voltages**
if the input voltages
are above or below the selected range.
Please ***[READ THE WARNINGS CAREFULLY](#warnings)!***

## Ports
- **A** and **B**:
    The inputs to the operators.

- **AND:**
  emits _min(A,B)_.

- **OR:**
  emits _max(A,B)_.

- **XOR:**
  emits _A+B-2*min(A,B)_.

- **A &#x25b6; B** (A implies B):
  emits _1-min(A,1-B)_.

- **A &#x25c0; B** (B implies A):
  emits _1-min(1-A,B)_.

- **¬ port**:
  Emits the negation of the operator it is grouped with.
  _NOT(x)_ is defined as _1-x_.

## Controls
- **UNI / BI:**
    The voltage range for which _Fuzzy Logic Z_ is well-behaved.
    0 to 10V (UNI)
    or -5 to 5V (BI).

    Input signals
    above or below the selected range
    **[can produce very large output votages](#warnings).**

    If the input signals are within the selected range,
    the output signals will also be within the selected range.

- **¬ button:**
  Negates the corresponding input
  before applying the logic operators.
  _NOT(x)_ is defined as _1-x_.

## Notes
- **Scaling the Inputs.**
  Before applying the operators,
  _Fuzzy Logic Z_ scales its inputs
  from the selected range
  to the "fuzzy logic" range \[0.0, 1.0\],
  where 0.0 means _completely false_,
  1.0 means _completely true_,
  and a value in between
  represents some degree of truth.
- **Scaling the Outputs.**
  After applying the fuzzy logic operators,
  _Fuzzy Logic Z_ scales each result
  to the range selected by the _UNI / BI_ switch.
- **Alternate Fuzzy Logic.**
  For fuzzy logic based on the less common "hyperbolic paraboloid" fuzzy operators,
  try **[_Fuzzy Logic H_]({{ 'modules/fuzzy-logic-H/'  | relative_url }} ).**

## Warnings

**_Fuzzy Logic Z_ can produce _VERY_ large output voltages
if the input are above or below
the the range selected by the _UNI / BI_ switch.**

- _Fuzzy Logic Z_
  is well-defined and well-behaved for input signals
  within in its selected range.
  If both input signals are in the selected range,
  all output signals will be in the selected range.
- Input signals above or below the selected range
  can produce large output signals.
- Input signals even _moderately_ above or below the selected range
  can produce ***VERY*** large output signals.
- Feeding _Fuzzy Logic Z_'s outputs
  back into its inputs
  can produce ***VERY*** large output signals
  if the inputs are only _slightly_
  above or below the selected range.
  
