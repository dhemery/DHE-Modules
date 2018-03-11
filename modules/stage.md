---
title: Stage Module
---
Generates a single stage of an envelope.

### Generating a Stage

A rising edge at the **TRIG** port
triggers the module to begin generating an envelope stage:
a signal that progresses from one voltage to another over time.
Stage emits the generated signal at its **OUT** port.

The stage starts at the voltage at the **IN** port
and progresses to the voltage specified by the **LEVEL** knob.
The LEVEL knob's range is 0–10V.
If the IN port is disconnected,
the stage starts at 0 volts.

The **DURATION** knob determines how long the stage takes
to progress from the start voltage to the end voltage.
In the center position,
the duration is 1 second.
At the fully clockwise position,
the duration is 10 seconds.
At the fully counterclockwise position,
the duration is 10 milliseconds.

The **CURVE** knob determines the shape of the stage signal.
At the center position, the CURVE knob produces a straight line
from the start voltage to the end voltage.

Rotating the CURVE knob clockwise produces a shape that,
in a rising stage,
appears J-shaped or "logarithmic" on a Scope.
The curve approaches the LEVEL voltage more slowly at first,
then more rapidly at the end.

Rotating the CURVE knob counterclockwise produces an inverted "logarithmic"
curve that approaches the LEVEL voltage more rapidly at first,
then more slowly at the end.

Rotating the CURVE knob further from the center in either direction
increases the curvature of the stage.
At extreme rotations,
the curvature is severe.

After its stage ends,
the module continues to emit the voltage of the LEVEL knob.

### Coordinating a Chain of Stages

Four of the ports
coordinate control in a chain of Stage modules.

The **TRIG** and **EOC** ports
allow each Stage to transfer control
to its downstream neighbor.

When a Stage module completes its stage,
it emits a short 10V pulse at its EOC port.
If you connect the EOC port to another Stage's TRIG port,
then whenever this Stage finishes generating,
its EOC pulse will trigger the next Stage to begin
generating its stage.

In this way, Stage modules hand control from one to the next.

The **DEFER** and **ACTIVE** ports
allow upstream Stages to wrest control
from all of the downstream Stages.

While a stage is in progress,
the **ACTIVE** port emits 10V.
