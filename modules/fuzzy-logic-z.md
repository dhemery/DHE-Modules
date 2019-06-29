---
title: The Fuzzy Logic Z Module
---
<img class="faceplate" src="fuzzy-logic-z.svg" alt="The Fuzzy Logic Z Faceplate" />

_Fuzzy Logic Z_ combines signals by applying ten fuzzy logic operators
based on Lofti Zadeh's definitions.

_Fuzzy Logic Z_ is arranged in two independent columns
with identical functionality.
Only the left column, which operates on the A and B inputs, is described below.
The right column applies the same operators to the C and D inputs.

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
- **¬ button:**
  Negates the corresponding input
  before applying the logic operators.
  _NOT(x)_ is defined as _1-x_.

## Notes
- **Scaling the Inputs.**
  Before applying the operators,
  _Fuzzy Logic Z_ divides each input by 10V
  to scale it to the "fuzzy logic" range \[0.0, 1.0\],
  where 0.0 means _completely false_,
  1.0 means _completely true_,
  and a value in between
  represents some degree of truth.
- **Scaling the Outputs.**
  After applying the fuzzy logic functions,
  _Fuzzy Logic Z_ multiplies each result by 10V
  to scale it to the "unipolar voltage" range \[0V, 10V\].
- **Alternate Fuzzy Logic.**
  For fuzzy logic based on the less common "hyperbolic paraboloid" fuzzy operators,
  try **[_Fuzzy Logic H_]({{ 'modules/fuzzy-logic-h/'  | relative_url }} ).**