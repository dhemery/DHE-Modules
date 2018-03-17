---
title: The Stage Module
---
<img class="panel" src="{% link /svg/stage.svg %}" alt="The Stage Panel" />

Generates a single-stage envelope.
Combine Stage modules
to generate complex envelopes
with any number of stages.

## Generating a Single-Stage Envelope

A rising edge at the **TRIG** port
triggers the module to begin generating an envelope stage:
a signal that progresses from one voltage to another over time.
Stage emits the generated envelope stage at its **OUT** port.

The envelope stage starts at the voltage at the **IN** port
and progresses to the voltage specified by the **LEVEL** knob.
The LEVEL knob's range is 0–10V.
If the IN port is disconnected,
the envelope stage starts at 0 volts.

The **DURATION** knob determines how long the envelope stage takes
to progress from the start voltage to the end voltage.
In the center position,
the duration is 1 second.
At the fully clockwise position,
the duration is 10 seconds.
At the fully counterclockwise position,
the duration is 10 milliseconds.

The **CURVE** knob determines the shape of the envelope stage.
At the center position, the CURVE knob produces a straight line
from the start voltage to the end voltage.

Rotating the CURVE knob clockwise produces a shape that,
in a rising envelope stage,
appears J-shaped or "logarithmic" on a Scope.
The curve approaches the LEVEL voltage more slowly at first,
then more rapidly at the end.

Rotating the CURVE knob counterclockwise produces an inverted "logarithmic"
curve that approaches the LEVEL voltage more rapidly at first,
then more slowly at the end.

Rotating the CURVE knob further from the center in either direction
increases the curvature of the envelope stage.
At extreme rotations,
the curvature is severe.

After its envelope stage ends,
Stage continues to emit the voltage of the LEVEL knob.

## Combining Stages to Generate Multi-Stage Envelopes

Four of the ports
coordinate control in a chain of Stage modules.

The **TRIG** and **EOC** ports
allow each Stage to transfer control
to its downstream neighbor.

When a Stage module completes its envelope stage,
it emits a short 10V pulse at its EOC port.
If you connect the EOC port to another Stage's TRIG port,
then whenever this Stage finishes generating,
its EOC pulse will trigger the next Stage to begin
generating its envelope stage.

In this way, Stage modules hand control from one to the next.

The **DEFER** and **ACTIVE** ports
allow upstream Stages to wrest control
from all of the downstream Stages.

While am envelope stage is in progress,
the **ACTIVE** port emits 10V.
