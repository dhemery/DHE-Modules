package panel

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	for _, s := range []scannibal{4, 8, 16} {
		registerBuilder(fmt.Sprintf("scannibal-%d", s), s.build)
	}
}

type scannibal int

func (s scannibal) build() *Panel {
	const (
		hue            = 30
		baseHp      Hp = 11
		stepWidthHp    = 2.25
	)
	var (
		fg = svg.HslColor(hue, 10, .1)
		bg = svg.HslColor(hue, .1, .93)
		hp = baseHp + Hp(float64(s)*stepWidthHp)
	)
	p := NewPanel(fmt.Sprintf("SCANNIBAL %d", s), hp, fg, bg, "scannibal")

	const (
		top                  = 23.0
		bottom               = 117.0
		globalInputsWidth    = padding + control.PortDiameter + padding
		stepBlockLabelsWidth = 10.0 + padding
		stepWidth            = stepWidthHp * mmPerHp

		portBoxAscent  = padding + svg.SmallFontSize + control.PortRadius + 0.22
		portBoxDescent = padding + control.PortRadius

		globalControlsTop    = top + portBoxAscent
		globalControlsBottom = bottom - portBoxDescent
		globalControlsDy     = (globalControlsBottom - globalControlsTop) / 4.0
	)
	var (
		width              = hp.toMM()
		stepBlockWidth     = float64(s) * stepWidth
		globalOutputsWidth = globalInputsWidth
		contentWidth       = globalInputsWidth + stepBlockLabelsWidth + stepBlockWidth + globalOutputsWidth
		margin             = (width - contentWidth) / 4.0
	)

	const (
		sequenceLengthY = globalControlsTop + 0.0*globalControlsDy
		aInputY         = globalControlsTop + 1.0*globalControlsDy
		bInputY         = globalControlsTop + 2.0*globalControlsDy
		cInputY         = globalControlsTop + 3.0*globalControlsDy
		phaseInY        = globalControlsTop + 4.0*globalControlsDy
	)
	var (
		globalInputsLeft  = margin
		globalInputsRight = globalInputsLeft + globalInputsWidth
		globalInputsX     = (globalInputsLeft + globalInputsRight) / 2.0
	)
	p.SmallKnob(globalInputsX, sequenceLengthY, "STEPS")
	p.InPort(globalInputsX, aInputY, "A")
	p.InPort(globalInputsX, bInputY, "B")
	p.InPort(globalInputsX, cInputY, "C")
	p.InPort(globalInputsX, phaseInY, "ϕ")

	const (
		intraSectionGlue = 0.15
		interSectionGlue = 2.0
		stepperWidth     = control.SmallKnobDiameter
	)
	var (
		anchorModes            = []string{"SMPL", "TRACK"}
		startAnchorModeStepper = control.Stepper("anchor-mode", fg, bg, svg.SmallFont, stepperWidth, 1, anchorModes)
		stepperAscent          = -startAnchorModeStepper.Top()
		stepperDescent         = startAnchorModeStepper.Bottom()

		startAnchorModeY    = top + stepperAscent
		startAnchorSourceY  = startAnchorModeY + stepperDescent + intraSectionGlue + stepperAscent
		startAnchorLevelY   = startAnchorSourceY + stepperDescent + intraSectionGlue + control.SmallKnobRadius
		startAnchorLevelCvY = startAnchorLevelY + control.SmallKnobRadius + intraSectionGlue + control.PortRadius

		endAnchorModeY    = startAnchorLevelCvY + control.PortRadius + interSectionGlue + stepperAscent
		endAnchorSourceY  = endAnchorModeY + stepperDescent + intraSectionGlue + stepperAscent
		endAnchorLevelY   = endAnchorSourceY + stepperDescent + intraSectionGlue + control.SmallKnobRadius
		endAnchorLevelCvY = endAnchorLevelY + control.SmallKnobRadius + intraSectionGlue + control.PortRadius

		shapeY       = endAnchorLevelCvY + control.PortRadius + interSectionGlue + stepperAscent
		curvatureY   = shapeY + stepperDescent + intraSectionGlue + control.SmallKnobRadius
		curvatureCvY = curvatureY + control.SmallKnobRadius + intraSectionGlue + control.PortRadius

		durationY   = curvatureCvY + control.PortRadius + interSectionGlue + control.SmallKnobRadius
		durationCvY = durationY + control.SmallKnobRadius + intraSectionGlue + control.PortRadius

		startAnchorLabelY = (startAnchorSourceY + startAnchorLevelCvY) / 2.0
		endAnchorLabelY   = (endAnchorSourceY + endAnchorLevelCvY) / 2.0
		shapeLabelY       = (shapeY + curvatureCvY) / 2.0
		durationLabelY    = (durationY + durationCvY) / 2.0
	)

	var (
		stepBlockLabelsLeft  = globalInputsRight + margin
		stepBlockLabelsRight = stepBlockLabelsLeft + stepBlockLabelsWidth
		stepBlockLabelsX     = stepBlockLabelsRight - padding
	)
	p.Engrave(stepBlockLabelsX, startAnchorLabelY, svg.TextLeft("ϕ＝0", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, endAnchorLabelY, svg.TextLeft("ϕ＝1", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, shapeLabelY, svg.TextLeft("SHAPE", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, durationLabelY, svg.TextLeft("[ ϕ ]", svg.LargeFont, fg))

	var (
		endAnchorModeStepper     = control.Stepper("anchor-mode", fg, bg, svg.SmallFont, stepperWidth, 2, anchorModes)
		anchorSources            = []string{"LEVEL", "A", "B", "C", "OUT"}
		startAnchorSourceStepper = control.Stepper("anchor-source", fg, bg, svg.SmallFont, stepperWidth, 5, anchorSources)
		endAnchorSourceStepper   = control.Stepper("anchor-source", fg, bg, svg.SmallFont, stepperWidth, 1, anchorSources)
		shapeNames               = []string{"J", "S"}
		shapeStepper             = control.Stepper("shape", fg, bg, svg.SmallFont, stepperWidth, 1, shapeNames)

		progressLightY = top - control.LightDiameter*1.5
		stepLabelY     = progressLightY - control.LightDiameter*1.5

		knob = control.SmallKnob(fg, bg)
		port = control.Port(fg, bg)

		stepBlockLeft  = stepBlockLabelsRight
		stepBlockRight = stepBlockLeft + stepBlockWidth
	)
	p.VLine(stepBlockLeft, top, bottom)
	for step := 0; step < int(s); step++ {
		left := stepBlockLeft + float64(step)*stepWidth
		right := left + stepWidth
		x := left + stepWidth/2.0
		p.VLine(right, top, bottom)

		p.Light(x, progressLightY)
		p.Engrave(x, stepLabelY, svg.TextAbove(fmt.Sprint(step+1), svg.LargeFont, fg))

		p.Install(x, startAnchorModeY, startAnchorModeStepper)
		p.Install(x, startAnchorSourceY, startAnchorSourceStepper)
		p.Install(x, startAnchorLevelY, knob)
		p.Install(x, startAnchorLevelCvY, port)

		p.Install(x, endAnchorModeY, endAnchorModeStepper)
		p.Install(x, endAnchorSourceY, endAnchorSourceStepper)
		p.Install(x, endAnchorLevelY, knob)
		p.Install(x, endAnchorLevelCvY, port)

		p.Install(x, shapeY, shapeStepper)
		p.Install(x, curvatureY, knob)
		p.Install(x, curvatureCvY, port)

		p.Install(x, durationY, knob)
		p.Install(x, durationCvY, port)
	}
	selectionMarkerOffset := (stepWidth-stepperWidth)/2.0 + control.LightRadius
	p.Install(stepBlockLeft+selectionMarkerOffset, progressLightY, control.SelectionMarker(0, fg))
	p.Install(stepBlockRight-selectionMarkerOffset, progressLightY, control.SelectionMarker(1, fg))

	var (
		globalOutputsLeft  = stepBlockRight + margin
		globalOutputsRight = globalOutputsLeft + globalOutputsWidth
		globalOutputsX     = (globalOutputsLeft + globalOutputsRight) / 2.0

		levelRangeY       = globalControlsTop + 0.0*globalControlsDy
		stepNumberOutputY = globalControlsTop + 2.0*globalControlsDy
		stepPhaseOutputY  = globalControlsTop + 3.0*globalControlsDy
		outY              = globalControlsTop + 4.0*globalControlsDy
	)
	p.LevelRangeSwitch(globalOutputsX, levelRangeY, 2)
	p.OutPort(globalOutputsX, stepNumberOutputY, "STEP #")
	p.OutPort(globalOutputsX, stepPhaseOutputY, "STEP ϕ")
	p.OutPort(globalOutputsX, outY, "OUT")

	return p
}
