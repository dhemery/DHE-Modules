---
title: The Cubic Module
---
<img class="panel" src="panel.svg" alt="The Cubic Panel" />

Transforms its input signal by offsetting, scaling, squaring, and cubing it.

*Cubic* calculates the function:

- *y* = *ax<sup>3</sup>* + *bx<sup>2</sup>* + *cx<sup>1</sup>* + *dx<sup>0</sup>*

which
(because *x<sup>1</sup>* = *x* and *x<sup>0</sup>* = 1)
can be written as:

- *y* = *ax<sup>3</sup>* + *bx<sup>2</sup>* + *cx* + *d*

## Controls
- **IN:**
    The gain to apply to the **IN** signal
    to produce the value of *x* for the equation.

- **x<sup>3</sup>:**
    The amount of *x* cubed to add to *y*
    (the *a* coefficient for the *ax<sup>3</sup>* term of the equation).

- **x<sup>2</sup>:**
    The amount of *x* squared to add to *y*
    (the *b* coefficient for the *bx<sup>2</sup>* term of the equation).

- **x<sup>1</sup>:**
    The amount of *x* to add to *y*
    (the *c* coefficient for the *cx<sup>1</sup>* term of the equation).
    This acts as a multiplier that scales *x*.

- **x<sup>0</sup>:**
    The constant to add to *y*
    (the *d* coefficient for the *dx<sup>0</sup>* term of the equation).
    This acts as an offset for *x*.

- **OUT:**
    The gain to apply to *y*
    before sending it to the **OUT** port.

## Ports
- **IN:**
    The input signal to transform.

- **OUT:**
    The transformed signal.

- **CV:**
    Description of port.
