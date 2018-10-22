---
title: The Xycloid Module
---
<img class="panel" src="panel.svg" alt="The Xycloid Panel" />

Produces a complex, pulsing
throb and wobble LFO effect
with accents that either repeat or vary.

See:
- **[Throb, Wobble, and Accent](https://www.youtube.com/watch?v=Kc0WctuFNvY):**
    My Xycloid tutorial on YouTube.
- **[The v0.6.1 manual](v0.6.1/):**
    More fun and less helpful than the current manual.

## Controls
- **RATIO:**
    The number of wobble cycles
    per throb cycle.

    **Note:**
    With the _IN / OUT_ switch
    in the center position,
    the _RATIO_ knob
    determines both the *rate* and *direction*
    in which the wobble angle advances.
    Clockwise rotation
    advances the wobble angle
    in the same direction
    as the throb angle.
    Counterclockwise rotation
    advances the wobble angle
    in the opposite direction.

- **FREE / LOCK:**
    Determines whether to round the _RATIO_ value
    to the nearest integer.

    _LOCK_ rounds the _RATIO_ value
    to the nearest integer,
    causing _Xycloid's_ pattern of accents
    to repeat on every throb cycle.

    _FREE_ allows non-integer _RATIO_ values,
    which lets the accents
    vary from one throb cycle to the next.

- **DEPTH:**
    The dominance of the wobble cycle in the output signals.

- **IN / OUT:**
    Determines the direction in which
    the wobble angle advances
    compared to the throb angle.

    _IN_ advances the wobble angle
    in the same direction
    as the throb angle.
    This produces "inward" nodes
    when the _X_ and _Y_ signals
    are plotted on an X/Y graph.

    _OUT_ advances the wobble angle
    in the opposite direction
    from the throb angle.
    This produces "outward" nodes
    when the _X_ and _Y_ signals
    are plotted on an X/Y graph.

    With this switch in the center position
    the _RATIO_ knob
    determines the direction
    in which the wobble angle advances.

- **SPEED:**
    The throb frequency.

- **PHASE:**
    Offsets the phase of the wobble angle.

    This has the effect
    of shifting the accents
    produced when the wobble and throb cycles
    peak.

## Ports
- **X OUT** and **Y OUT:**
    The output signals.

    The polarity switch **(UNI / BI)**
    selects the voltage range of the associated output signal.
    0 to 10V (UNI)
    or -5 to 5V (BI).

    The **GAIN** knob applies gain (from 0 to 2)
    to the signal.

    _Xycloid_ selects the range before applying the gain.

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

## Menu Options

- **Musical Ratios:**
    Determines the range of the _RATIO_ knob.

    When checked
    the maximum wobble-to-throb ratio is 16.

    When unchecked
    the maximum wobble-to-throb ratio is either 15 or 17,
    depending on the position of the _IN / OUT_ switch.

## Using Xycloid

1. Choose one or two Rack modules that you want to modulate.
1. Choose two parameters that have CV modulation.
1. Connect Xycloid's _X_ output to one parameter's CV input
    and the _Y_ output to the other's.
1. (Optional)
   To view the shape that Xycloid produces,
   connect its _X_ and _Y_ outputs to a Scope's _X_ and _Y_ inputs.
   Set the scope to _X x Y_ mode.
   (_X/Y_ mode can also be interesting.)
1. **Play with the knobs.**
1. For extra chaos,
    modulate Xycloid's controls.
1. For extra, extra chaos,
    use Xycloid's outputs
    to modulate its own controls.

**The Audible Instruments Macro Oscillator**
is a fantastic choice for exploring what Xycloid can do.
Attach Xycloid's _X_ and _Y_ outputs
to the Macro Oscillator's _TIMBRE_ and _COLOR_ inputs.
Modulating _V/OCT_ and _FM_ is also fun.

Then **play with the knobs.**

**More stuff to try:**

- Modulate two closely related parameters.
- Modulate two parameters that have nothing to do with each other.
- Modulate the same parameter on different modules.
    For example,
    modulate the _PWM_ input on two Fundamental VCO-1s.
    Or the _FM_ inputs.

## Plotting Xycloid

When plotted on a two-dimensional graph,
the _X_ and _Y_ signals
produce a [visually pleasing shape](#examples),
which can be either a
_[hypotrochoid](https://en.wikipedia.org/wiki/Hypotrochoid)_
or an
_[epitrochoid](https://en.wikipedia.org/wiki/Epitrochoid)._
