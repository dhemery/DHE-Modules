package panel

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	for _, s := range []sequencizer{4, 8, 16} {
		registerBuilder(fmt.Sprintf("sequencizer-%d", s), s.build)
	}
}

type sequencizer int

func (s sequencizer) build() *Panel {
	const (
		hue            = 30
		baseHp      Hp = 15
		stepWidthHp    = 2.25
	)
	var (
		fg = svg.HslColor(hue, 10, .1)
		bg = svg.HslColor(hue, .1, .93)
		hp = baseHp + Hp(float64(s)*stepWidthHp)
	)
	p := NewPanel(fmt.Sprintf("SEQUENCIZER %d", s), hp, fg, bg, "sequencizer")

	const (
		top       = 23.0
		bottom    = 117.0
		stepWidth = stepWidthHp * mmPerHp

		portBoxWidth       = padding + control.PortDiameter + padding
		portBoxAscent      = control.PortRadius + padding + svg.SmallFontSize + padding
		portBoxDescent     = control.PortRadius + padding
		buttonPortBoxWidth = padding + control.PortDiameter + padding + control.ButtonDiameter + padding

		sequenceBlockWidth           = buttonPortBoxWidth
		globalStepControlsBlockWidth = portBoxWidth + padding + portBoxWidth + padding + portBoxWidth
		stepBlockLabelsWidth         = 10.0 + padding

		globalControlsTop    = top + portBoxAscent
		globalControlsBottom = bottom - portBoxDescent
		globalControlsDy     = (globalControlsBottom - globalControlsTop) / 4.0
	)
	var (
		width          = hp.toMM()
		stepBlockWidth = float64(s) * stepWidth
		contentWidth   = sequenceBlockWidth + globalStepControlsBlockWidth + stepBlockLabelsWidth + stepBlockWidth
		margin         = (width - contentWidth) / 4.0
	)

	var (
		sequenceBlockLeft  = margin
		sequenceBlockRight = sequenceBlockLeft + sequenceBlockWidth
		sequenceControlsX  = sequenceBlockLeft + padding + control.PortRadius
		selectionStartX    = sequenceBlockLeft + control.SmallKnobRadius
		selectionLengthX   = sequenceBlockRight - control.SmallKnobRadius
	)
	y := globalControlsTop
	p.InButtonPort(sequenceControlsX, y, "RUN")
	y += globalControlsDy
	p.InButtonPort(sequenceControlsX, y, "LOOP")
	y += globalControlsDy
	p.HLine(selectionStartX, selectionLengthX, y)
	p.SmallKnob(selectionStartX, y, "START")
	p.SmallKnob(selectionLengthX, y, "LEN")
	y += globalControlsDy
	p.InButtonPort(sequenceControlsX, y, "GATE")
	y += globalControlsDy
	p.InButtonPort(sequenceControlsX, y, "RESET")

	const (
		globalStepControlsDx = portBoxWidth + padding
	)
	var (
		globalStepControlsBlockLeft  = sequenceBlockRight + margin
		globalStepControlsBlockRight = globalStepControlsBlockLeft + globalStepControlsBlockWidth
		globalStepControlsLeftX      = globalStepControlsBlockLeft + portBoxWidth/2.0
		globalStepControlsCenterX    = globalStepControlsLeftX + 1.0*globalStepControlsDx
		globalStepControlsRightX     = globalStepControlsLeftX + 2.0*globalStepControlsDx
	)
	y = globalControlsTop
	p.HLine(globalStepControlsLeftX, globalStepControlsRightX, y)
	p.CvPort(globalStepControlsLeftX, y)
	p.SmallKnob(globalStepControlsCenterX, y, "LEVEL")
	p.LevelRangeSwitch(globalStepControlsRightX, y, 1)

	y += globalControlsDy
	p.HLine(globalStepControlsLeftX, globalStepControlsRightX, y)
	p.CvPort(globalStepControlsLeftX, y)
	p.SmallKnob(globalStepControlsCenterX, y, "DUR")
	p.DurationRangeSwitch(globalStepControlsRightX, y, 2)

	y += globalControlsDy
	p.InPort(globalStepControlsLeftX, y, "A")
	p.InPort(globalStepControlsCenterX, y, "B")
	p.InPort(globalStepControlsRightX, y, "C")

	y += globalControlsDy
	p.OutPort(globalStepControlsLeftX, y, "STEP #")
	p.OutPort(globalStepControlsCenterX, y, "CURVE")
	p.OutPort(globalStepControlsRightX, y, "SUST")

	y += globalControlsDy
	p.OutPort(globalStepControlsLeftX, y, "STEP Δ")
	p.OutPort(globalStepControlsCenterX, y, "SEQ Δ")
	p.OutPort(globalStepControlsRightX, y, "OUT")

	const (
		intraSectionGlue = 0.5
		interSectionGlue = 4.0
		stepperWidth     = control.SmallKnobDiameter
	)
	var (
		interruptModes       = []string{"OFF", "ON"}
		interruptModeStepper = control.Stepper("interrupt-mode", fg, bg, svg.SmallFont, stepperWidth, 1, interruptModes)

		stepperAscent  = -interruptModeStepper.Top()
		stepperDescent = interruptModeStepper.Bottom()

		triggerModeY   = top + stepperAscent
		interruptModeY = triggerModeY + stepperDescent + intraSectionGlue + stepperAscent
		sustainModeY   = interruptModeY + stepperDescent + intraSectionGlue + stepperAscent

		startAnchorModeY   = sustainModeY + stepperDescent + interSectionGlue + stepperAscent
		startAnchorSourceY = startAnchorModeY + stepperDescent + intraSectionGlue + stepperAscent
		startAnchorLevelY  = startAnchorSourceY + stepperDescent + intraSectionGlue + control.SmallKnobRadius

		endAnchorModeY   = startAnchorLevelY + control.SmallKnobRadius + interSectionGlue + stepperAscent
		endAnchorSourceY = endAnchorModeY + stepperDescent + intraSectionGlue + stepperAscent
		endAnchorLevelY  = endAnchorSourceY + stepperDescent + intraSectionGlue + control.SmallKnobRadius

		shapeY     = endAnchorLevelY + control.SmallKnobRadius + interSectionGlue + stepperAscent
		curvatureY = shapeY + stepperDescent + intraSectionGlue + control.SmallKnobRadius

		durationY = curvatureY + control.SmallKnobRadius + interSectionGlue + control.SmallKnobRadius

		enabledY = durationY + control.SmallKnobRadius + interSectionGlue + control.ButtonRadius

		startAnchorLabelY = (startAnchorModeY + startAnchorLevelY) / 2.0
		endAnchorLabelY   = (endAnchorModeY + endAnchorLevelY) / 2.0
		shapeLabelY       = (shapeY + curvatureY) / 2.0
	)

	var (
		stepBlockLabelsLeft  = globalStepControlsBlockRight + margin
		stepBlockLabelsRight = stepBlockLabelsLeft + stepBlockLabelsWidth
		stepBlockLabelsX     = stepBlockLabelsRight - padding
	)
	p.Engrave(stepBlockLabelsX, triggerModeY, svg.TextLeft("TRIG", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, interruptModeY, svg.TextLeft("INT", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, sustainModeY, svg.TextLeft("SUST", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, startAnchorLabelY, svg.TextLeft("START", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, endAnchorLabelY, svg.TextLeft("END", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, durationY, svg.TextLeft("DUR", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, shapeLabelY, svg.TextLeft("SHAPE", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelsX, enabledY, svg.TextLeft("ON", svg.LargeFont, fg))

	var (
		anchorModes              = []string{"SMPL", "TRACK"}
		endAnchorModeStepper     = control.Stepper("anchor-mode", fg, bg, svg.SmallFont, stepperWidth, 2, anchorModes)
		anchorSources            = []string{"LEVEL", "A", "B", "C", "OUT"}
		endAnchorSourceStepper   = control.Stepper("anchor-source", fg, bg, svg.SmallFont, stepperWidth, 1, anchorSources)
		shapeNames               = []string{"J", "S"}
		shapeStepper             = control.Stepper("shape", fg, bg, svg.SmallFont, stepperWidth, 1, shapeNames)
		startAnchorModeStepper   = control.Stepper("anchor-mode", fg, bg, svg.SmallFont, stepperWidth, 1, anchorModes)
		startAnchorSourceStepper = control.Stepper("anchor-source", fg, bg, svg.SmallFont, stepperWidth, 5, anchorSources)
		sustainModes             = []string{"OFF", "ON"}
		sustainModeStepper       = control.Stepper("sustain-mode", fg, bg, svg.SmallFont, stepperWidth, 1, sustainModes)
		triggerModes             = []string{"RISE", "FALL", "EDGE", "HIGH", "LOW"}
		triggerModeStepper       = control.Stepper("trigger-mode", fg, bg, svg.SmallFont, stepperWidth, 1, triggerModes)

		button = control.Button(bg, fg)
		knob   = control.SmallKnob(fg, bg)

		stepBlockLeft  = stepBlockLabelsX + padding
		stepBlockRight = stepBlockLeft + stepBlockWidth
		progressLightY = top - control.LightDiameter*1.5
		stepLabelY     = progressLightY - control.LightDiameter*1.5
	)
	p.VLine(stepBlockLeft, top, bottom)
	for step := 0; step < int(s); step++ {
		left := stepBlockLeft + float64(step)*stepWidth
		right := left + stepWidth
		x := left + stepWidth/2.0
		p.VLine(right, top, bottom)

		p.Light(x, progressLightY)
		p.Engrave(x, stepLabelY, svg.TextAbove(fmt.Sprint(step+1), svg.LargeFont, fg))

		p.Install(x, triggerModeY, triggerModeStepper)
		p.Install(x, interruptModeY, interruptModeStepper)
		p.Install(x, sustainModeY, sustainModeStepper)

		p.Install(x, startAnchorModeY, startAnchorModeStepper)
		p.Install(x, startAnchorSourceY, startAnchorSourceStepper)
		p.Install(x, startAnchorLevelY, knob)

		p.Install(x, endAnchorModeY, endAnchorModeStepper)
		p.Install(x, endAnchorSourceY, endAnchorSourceStepper)
		p.Install(x, endAnchorLevelY, knob)

		p.Install(x, shapeY, shapeStepper)
		p.Install(x, curvatureY, knob)
		p.Install(x, durationY, knob)
		p.Install(x, enabledY, button)
	}
	selectionMarkerOffset := (stepWidth-stepperWidth)/2.0 + control.LightRadius
	p.Install(stepBlockLeft+selectionMarkerOffset, progressLightY, control.SelectionMarker(0, fg))
	p.Install(stepBlockRight-selectionMarkerOffset, progressLightY, control.SelectionMarker(1, fg))

	return p
}
