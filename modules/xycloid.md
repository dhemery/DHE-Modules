---
title: The Xycloid Module
---
<img class="panel" src="panel.svg" alt="The Xycloid Panel" />

Generates cyclically related pairs of sinusoidal signals.

Uh… what?

Yeah, don't worry about all that.

_Xycloid_ [makes fun shapes](#my-inspiration).

Just play with the knobs.

## Controls
- **RATIO:**
    Determines the number of "cusps" or "nodes" in the shape.

    The "curl direction" switch **(IN / OUT)**
    determines whether the cusps curl
    toward the inside or the outside of the shape.
    With the switch at the top or bottom position,
    the *RATIO* knob ranges from 0 to 16 cusps.
    With the switch in the center,
    the *RATIO* knob ranges from 16 inward cusps
    (fully counterclockwise)
    to 16 outward cusps
    (fully clockwise).

- **DEPTH:**
    How far the shape extends from its outer edge.

    Note that with the knob at either extreme,
    the shape is a circle.

- **SPEED:**
    The speed at which the shape is drawn.
    Turn the knob counterclockwise to reverse the shape's direction.

    Note that the **RATIO** knob
    (and to some extent the **DEPTH** knob)
    can also affect the speed
    in counterintuitive ways.

## Ports
- **X OUT and Y OUT:**
    The output signals.

    The polarity switch **(UNI / BI)**
    selects the range of the signal:
    Either 0 to 10V (UNI)
    or -5 to 5V (BI).

    The **GAIN** knob applies gain (from -2 to 2)
    to the signal.
    The amount of gain is modulated by a _CV_ signal.

    The range is selected before gain is applied.

- **CV:**
    Control voltage signal that modulates the associated knob.
    The modulation is calibrated so that:
    - A 0V _CV_ signal yields the value of the knob.
    - A 5V _CV_ signal yields the value
        as if the knob were rotated 50%
        clockwise
        of its actual position.
    - A -5V _CV_ signal yields the value
        as if the knob were rotated 50%
        counterclockwise
        of its actual position.

    Some of the _CV_ ports
        are accompanied by a small *attenuverter* (**- +**) knob.
        This knob adjusts the strength and direction
        of the the _CV_ signal's effect
        on the modulated knob.

## My Inspiration

Okay, so remember when you were a kid,
and [you had a toy](https://en.wikipedia.org/wiki/Spirograph)
where you stuck a pen in a hole in a little plastic gear
and you rolled the gear around inside a plastic ring
to draw fun shapes?

That's what _Xycloid_ does.

If you want to know the math,
look up
[epitrochoid](https://en.wikipedia.org/wiki/Epitrochoid)
and
[hypotrochoid](https://en.wikipedia.org/wiki/Hypotrochoid).

But, seriously, don't worry about that.

Just play with the knobs.

## The Name

There is a kind of shape called a _cycloid._
Technically,
_Xycloid_ doesn't draw those.
Well,
technically,
it does.
Sort of.

But more preciesely,
_Xycloid_ draws [centered trochoids](https://en.wikipedia.org/wiki/Centered_trochoid).

So why did I call the module _Xycloid?_

Because would you load a module called _Centered Trochoid?_

No.

No, you would not.

But don't worry about that.

Just play with the knobs.
