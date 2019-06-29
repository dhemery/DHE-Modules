---
title: The Fuzzy Logic H Module
---
<img class="faceplate" src="fuzzy-logic-h.svg" alt="The Fuzzy Logic H Faceplate" />

_Fuzzy Logic H_ combines signals by applying ten fuzzy logic operators
based on hyperbolic paraboloids.

_Fuzzy Logic H_ is arranged in two independent columns
with identical functionality.
Only the left column, which operates on the A and B inputs, is described below.
The right column applies the same operators to the C and D inputs.

**WARNING:**
_Fuzzy Logic H_
can emit **very large voltages.**
Before using _Fuzzy Logic H_
with audio signals,
before feeding _Fuzzy Logic H_'s outputs
back into its inputs,
**[read the warnings carefully](#warnings).**

## Ports
- **A** and **B**:
    The inputs to the operators.

- **AND:**
  emits _A*B_.

- **OR:**
  emits _A+B-(A*B)_.

- **XOR:**
  emits _A+B-(2*A*B)_.

- **A &#x25b6; B** (A implies B):
  emits _1-A+(A*B)_.

- **A &#x25c0; B** (B implies A):
  emits _1-B+(A*B)_.

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
  _Fuzzy Logic H_ divides each input by 10V
  to scale it to the "fuzzy logic" range \[0.0, 1.0\],
  where 0.0 means _completely false_,
  1.0 means _completely true_,
  and a value in between
  represents some degree of truth.
- **Scaling the Outputs.**
  After applying the fuzzy logic functions,
  _Fuzzy Logic H_ multiplies each result by 10V
  to scale it to the "unipolar voltage" range \[0V, 10V\].
- **Alternate Fuzzy Logic.**
  For fuzzy logic based on Lofti Zadeh's more standard definitions of the same operators,
  try **[_Fuzzy Logic Z_]({{ 'modules/fuzzy-logic-z/'  | relative_url }} ).**
  
  
## Warnings

**_Fuzzy Logic H_ can produce _VERY_ large output voltages
if given input voltages above 10V or below 0V.**

- _Fuzzy Logic H_
  is well-defined and well-behaved for input signals
  between 0V and 10V.
- Input signals above 10V or below 0V
  can produce large output signals.
- Input signal even _moderately_ above 10V or below 0V
  can produce ***VERY*** large output signals.
- Feeding _Fuzzy Logic H_'s outputs
  back into its inputs
  can produce ***VERY*** large output signals
  if the inputs are only _slightly_
  above 10V or below 0V. 

**Using _Fuzzy Logic H_ with audio signals.**
Audio signals fall outside of _Fuzzy Logic H_'s
"well-behaved" input range.
If you want send audio signals to _Fuzzy Logic H_'s inputs,
consider:
- Before sending an audio signal to _Fuzzy Logic H_,
  offset it by +5V
  to translate it into the range \[0V, 10V\].
- Note that some audio signals peak above 5V or below -5V.
  You may need to attenuate such signals
  or otherwise adjust them
  before sending them into _Fuzzy Logic H_.
- Offset each _Fuzzy Logic H_ output signal by -5V
  to translate it into audio range. 
