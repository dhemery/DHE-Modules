---
title: Release Notes
---

# Version 2.0.0
November 2021

- Updated all modules to be compatible with VCV Rack 2.
- Hid the  [_Curve Sequencer_](/modules/curve-sequencer) modules from the module browser.
    Use [_Sequencizer_](/modules/sequencizer/) instead.

# Version 1.5.1
November 2020

- Updated the _Truth_ modules
    -   Changed the mechanism for making a truth table depend on its previous state (Q).
        This change removes the _A/Q_ button from column 1
        and gives each result button two additional options: _Q_ and _¬Q._
        This allows creating a state-dependent truth table
        with fewer columns.
    -   Taught the modules how to upgrade existing presets
        to use the new result options instead of the _A/Q_ button.
    -   Added override buttons to the _Q_ and _¬Q_ outputs.
    -   Moved each preset to the next lower-ranked _Truth_ module.
    -   Added a _Truth 4_ preset that implements a D latch with set and reset.  

# Version 1.5.0
November 2020

-   Added _Truth 2_.
-   Added [_Truth 3_](/modules/truth/).
-   Added _Truth 4_.
-   Added [_Buttons_](/modules/buttons/).

# Version 1.4.0
November 2020

-   Added [_Scannibal 4_](/modules/scannibal/).
-   Added _Scannibal 8_.
- Added _Scannibal 16_.

# Version 1.3.0
October 2020

-   Added [_Sequencizer 4_](/modules/sequencizer/).
-   Added _Sequencizer 8_.
-   Added _Sequencizer 16_.
-   Changed the presentation of "stepper" controls.

# Version 1.2.0
December 2019

-   Added three new modes to the Curve Sequencer modules:
    -   _INPUT_ mode copies the _IN_ signal to the _OUT_ port.
    -   _LEVEL_ mode copies the _LEVEL_ voltage to the _OUT_ port.
    -   _CURVE_ mode generates a curve that gradually approaches the _IN_ signal.

# Version 1.1.1
November 2019

-   Fixed an error in the _Ranger_ GUI.

# Version 1.1.0
November 2019

-   Added [_Curve Sequencer 4_](/modules/curve-sequencer).
-   Added _Curve Sequencer 8_.
-   Added _Curve Sequencer 16_.

# Version 1.0.5
August 2019

-   _Stage,_ _Booster Stage,_ and _Hostage._
    -   Fixed a defect that prevented these modules from retriggering
        while a stage was in progress.

# Version 1.0.4
July 2019

-   _Fuzzy Logic H_ and  _Fuzzy Logic Z_
    -   Added a _UNI / BI_ switch
        to allow operating
        on audio and other bipolar signals (-5V to 5V).
    -   The _OR_ and _NOR_ output port widgets for inputs _C_ and _D_
        were incorrectly connected to the _XOR_ and _XNOR_ outputs.

# Version 1.0.3
June 2019

-   Added [_Fuzzy Logic H_](/modules/fuzzy-logic-h).
-   Added [_Fuzzy Logic Z_](/modules/fuzzy-logic-z).

# Version 1.0.2
June 2019

-   Added [_Gator_](/modules/gator).

# Version 1.0.1
June 2019

-   Xycloid
    -   Added phase CV with attenuverter.
    -   Reordered controls to match Blossom.
-   Swave
    -   Added a CV attenuverter.

# Version 1.0.0
May 2019

-   All modules
    -   Added parameter tooltips.
    -   Added the ability to specify parameter values via keyboard.
-   Ranger
    -   Renamed and reoriented the limit controls to better convey their effects on the **LEVEL** knob.
-   Swave
    -   Restored the shape switch.

# Release 0.6.5
March 2019

-   Added [_Blossom_](/modules/blossom).
-   Fixed a defect that caused the envelope generator modules
    to sometimes generate a single sample of the wrong voltage.
    When used to modulate volume,
    the result was a very nasty click.
    A huge thank you
    to Omri Cohen for finding and reporting this problem.

# Release 0.6.4
January 2019

-   Added [_Func_](/modules/func).
-   Added [_Func 6_](/modules/func-6).
-   Significantly improved performance of the envelope generator modules.

# Release 0.6.3
November 2018

-   Added [_Ranger_](/modules/ranger).
-   Added [_Tapers_](/modules/tapers).

# Release 0.6.2
October 2018

-   Added a _PHASE_ knob and a _RATIO_ quantizer switch
    to [_Xycloid_](/modules/xycloid).

# Release 0.6.1
July 2018

-   Added [_Cubic_](/modules/cubic).
-   Added [_Xycloid_](/modules/xycloid).

# Release 0.6.0
March 2018

-   Added [_Booster Stage_](/modules/booster-stage).
-   Added [_Hostage_](/modules/hostage).
-   Added [_Swave_](/modules/swave).

# Release 0.5.0

February 2018
-   Added [_Stage_](/modules/stage).
-   Added [_Upstage_](/modules/upstage).
