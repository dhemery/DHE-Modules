---
title: The Flogic Module
---
<img class="faceplate" src="flogic.svg" alt="The Flogic Faceplate" />

_Flogic_ computes eight fuzzy logic functions.

_Flogic_ has two independent panels
with identical functionality.

**Definitions:**

- **NOT(x)** means _1 - x_.
  The ¬ symbol means _NOT_. 
- **AND(x,y)** means _x * y_.
- **OR(x, y)** means _1 - (1 - x) * (1 - x)_.
  
## Ports
- **A** and **B**:
    The fuzzy logic inputs.

- **AND:**
  A * B.

- **OR:**
  1 - (1 - A) * (1 - B).
  
- **MAX:**
  The larger of A and B.

- **MIN:**
  The smaller of A and B.

- **¬ port**:
  Emits the negation of the function it is grouped with.

## Controls
- **¬ button:**
  Negates the corresponding input
  before computing the logic functions.

## Notes
- **Scaling the Inputs.**
  Before applying the fuzzy logic functions,
  _Flogic_ divides each input by 10V
  to scale it to the "fuzzy logic" range \[0.0, 1.0\],
  where 0.0 means _completely false_,
  1.0 means _completely true_,
  and a value in between
  represents some degree of truth.
- **Scaling the Outputs.**
  After applying the fuzzy logic functions,
  _Flogic_ multiplies each result by 10V
  to scale it to the "unipolar voltage" range \[0V, 10V\].
- **Definition of _OR._**
  Though the definition of _OR_ may seem arbitrary,
  it follows from our definitions of
  _AND_ and _NOT_.
  To see how it follows,
  recall DeMorgan's Theorem: _OR(x,y) = ¬AND(¬x, ¬y)_.
- **Alternate Definitions.**
  Some logic systems
  define _AND(x,y) = max(x,y)_
  and _OR(x,y) = min(x,y)_.
- **Beyond Truth.**
  _Flogic_ is perfectly safe with signals outside the range \[0V, 10V\].
  Do such signals have any meaning in fuzzy logic?
  Who knows.
  A better question:
  Are they musically useful?
