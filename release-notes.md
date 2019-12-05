---
title: Release Notes
---

# DHE Modules Release Notes

## Version 1.1.1

- Fixed an error in the _Ranger_ GUI.

## Version 1.1.0

- Added [_Curve Sequencer 4_]({{ '/modules/curve-sequencer' | relative_url }}).
- Added _Curve Sequencer 8_.
- Added _Curve Sequencer 16_.

## Version 1.0.5

- _Stage,_ _Booster Stage,_ and _Hostage._
  - Fixed a defect that prevented these modules from retriggering
    while a stage was in progress.

## Version 1.0.4

- _Fuzzy Logic H_ and  _Fuzzy Logic Z_
  - Added a _UNI / BI_ switch
    to allow operating
    on audio and other bipolar signals (-5V to 5V).
  - The _OR_ and _NOR_ output port widgets for inputs _C_ and _D_
    were incorrectly connected to the _XOR_ and _XNOR_ outputs.

## Version 1.0.3

- Added [_Fuzzy Logic H_]({{ '/modules/fuzzy-logic-h' | relative_url }}).
- Added [_Fuzzy Logic Z_]({{ '/modules/fuzzy-logic-z' | relative_url }}).

## Version 1.0.2

- Added [_Gator_]({{ '/modules/gator' | relative_url }}).

## Version 1.0.1

- Xycloid
  - Added phase CV with attenuverter.
  - Reordered controls to match Blossom.
- Swave
  - Added a CV attenuverter.

## Version 1.0.0

- All modules
  - Added parameter tooltips.
  - Added the ability to specify parameter values via keyboard.
- Ranger
  - Renamed and reoriented the limit controls to better convey their effects on the **LEVEL** knob.
- Swave
  - Restored the shape switch.

## Release 0.6.5

- Added [_Blossom_]({{ '/modules/blossom' | relative_url }}).
- Fixed a defect that caused the envelope generator modules
  to sometimes generate a single sample of the wrong voltage.
  When used to modulate volume,
  the result was a very nasty click.
  A huge thank you
  to Omri Cohen for finding and reporting this problem.

## Release 0.6.4

- Added [_Func_]({{ '/modules/func' | relative_url }}).
- Added [_Func 6_]({{ '/modules/func-6' | relative_url }}).
- Significantly improved performance of the envelope generator modules. 

## Release 0.6.3
- Added [_Ranger_]({{ '/modules/ranger' | relative_url }}).
- Added [_Tapers_]({{ '/modules/tapers' | relative_url }}).

## Release 0.6.2
- Added a _PHASE_ knob and a _RATIO_ quanitizer switch
  to [_Xycloid_]({{ '/modules/xycloid' | relative_url }}).

## Release 0.6.1
- Added [_Cubic_]({{ '/modules/cubic' | relative_url }}).
- Added [_Xycloid_]({{ '/modules/xycloid' | relative_url }}).

## Release 0.6.0
- Added [_Booster Stage_]({{ '/modules/booster-stage' | relative_url }}).
- Added [_Hostage_]({{ '/modules/hostage' | relative_url }}).
- Added [_Swave_]({{ '/modules/swave' | relative_url }}).

## Release 0.5.0
- Added [_Stage_]({{ '/modules/stage' | relative_url }}).
- Added [_Upstage_]({{ '/modules/upstage' | relative_url }}).