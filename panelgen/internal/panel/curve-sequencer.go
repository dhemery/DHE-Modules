package panel

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	for _, cs := range []curveSequencer{4, 8, 16} {
		registerBuilder(cs.slug(), cs.build)
	}
}

type curveSequencer int

func (cs curveSequencer) slug() string {
	return fmt.Sprintf("curve-sequencer-%d", cs)
}

func (cs curveSequencer) build() *Panel {
	const (
		hue         = 30
		baseHp      = 13
		stepWidthHp = 2.25
	)
	var (
		steps    = int(cs)
		hp    Hp = baseHp + Hp(float64(steps)*stepWidthHp)
		fg       = svg.HslColor(hue, 10, .1)
		bg       = svg.HslColor(hue, .1, .93)
	)
	p := NewPanel(fmt.Sprintf("CURVE SEQUENCER %d", cs), hp, fg, bg, "curve-sequencer")

	const (
		left                   = 2.0 * mmPerHp
		top                    = 4.0 * mmPerHp
		bottom                 = 23.0 * mmPerHp
		sequenceControlsTop    = top + 2.75*mmPerHp
		sequenceControlsBottom = bottom - control.PortRadius - 1.0
		sequenceControlsDy     = (sequenceControlsBottom - sequenceControlsTop) / 4.0
		runY                   = sequenceControlsTop + 0.0*sequenceControlsDy
		loopY                  = sequenceControlsTop + 1.0*sequenceControlsDy
		selectionY             = sequenceControlsTop + 2.0*sequenceControlsDy
		gateY                  = sequenceControlsTop + 3.0*sequenceControlsDy
		resetY                 = sequenceControlsTop + 4.0*sequenceControlsDy
		selectionStartX        = left - control.PortRadius - padding + control.SmallKnobRadius
		selectionLengthX       = left + control.PortRadius + padding + control.ButtonDiameter + padding - control.SmallKnobRadius
	)
	p.InButtonPort(left, runY, "RUN")
	p.InButtonPort(left, gateY, "GATE")
	p.SmallKnob(selectionStartX, selectionY, "START")
	p.SmallKnob(selectionLengthX, selectionY, "LEN")
	p.HLine(selectionStartX, selectionLengthX, selectionY)
	p.InButtonPort(left, loopY, "LOOP")
	p.InButtonPort(left, resetY, "RESET")

	const (
		stepWidth       = stepWidthHp * mmPerHp
		stepBlockLeft   = 10.0*mmPerHp - stepWidth/2.0
		stepBlockLabelX = stepBlockLeft - 0.1*stepWidth

		generateModeY  = top + 1.61*mmPerHp
		advanceModeY   = top + 3.25*mmPerHp
		levelY         = top + 5.75*mmPerHp
		shapeY         = top + 8.75*mmPerHp
		curveY         = top + 11.0*mmPerHp
		durationY      = top + 14.0*mmPerHp
		enabledPortY   = bottom - control.PortRadius
		enabledButtonY = enabledPortY - control.PortRadius - control.ButtonRadius - 1.0
	)
	p.Engrave(stepBlockLabelX, generateModeY, svg.TextLeft("GENERATE", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelX, advanceModeY, svg.TextLeft("ADVANCE", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelX, levelY, svg.TextLeft("LEVEL", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelX, shapeY, svg.TextLeft("SHAPE", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelX, curveY, svg.TextLeft("CURVE", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelX, durationY, svg.TextLeft("DURATION", svg.LargeFont, fg))
	p.Engrave(stepBlockLabelX, (enabledButtonY+enabledPortY)/2.0, svg.TextLeft("ENABLED", svg.LargeFont, fg))

	const (
		stepLabelY          = top - 0.5*mmPerHp
		activeY             = top + control.LightRadius
		channelSeparatorTop = top + 1.25*mmPerHp
		stepperWidth        = 9.0
	)
	var (
		stepBlockRight = stepBlockLeft + float64(steps)*stepWidth

		smallKnob           = control.SmallKnob(fg, bg)
		port                = control.Port(fg, bg)
		button              = control.Button(bg, fg)
		advanceModes        = []string{"TIME", "RISE", "FALL", "EDGE", "HIGH", "LOW"}
		advanceModeStepper  = control.Stepper("advance-mode", fg, bg, svg.SmallFont, stepperWidth, 1, advanceModes)
		generateModes       = []string{"CURVE", "HOLD", "SUST", "INPUT", "CHASE", "LEVEL"}
		generateModeStepper = control.Stepper("generate-mode", fg, bg, svg.SmallFont, stepperWidth, 1, generateModes)
	)
	for step := 0; step < steps; step++ {
		channelSeparatorX := stepBlockLeft + float64(step)*stepWidth
		p.VLine(channelSeparatorX, channelSeparatorTop, bottom)
		x := channelSeparatorX + stepWidth/2.0
		p.Light(x, activeY)
		p.Engrave(x, stepLabelY, svg.TextAbove(fmt.Sprint(step+1), svg.LargeFont, fg))
		p.Install(x, generateModeY, generateModeStepper)
		p.Install(x, advanceModeY, advanceModeStepper)
		p.ShapeSwitch(x, shapeY, 1)
		p.Install(x, curveY, smallKnob)
		p.Install(x, levelY, smallKnob)
		p.Install(x, durationY, smallKnob)
		p.Install(x, enabledButtonY, button)
		p.Install(x, enabledPortY, port)
		p.VLine(x, enabledButtonY, enabledPortY)
	}

	selectionMarkerOffset := (stepWidth-stepperWidth)/2.0 + control.LightRadius
	p.Install(stepBlockLeft+selectionMarkerOffset, activeY, control.SelectionMarker(0, fg))
	p.Install(stepBlockRight-selectionMarkerOffset, activeY, control.SelectionMarker(1, fg))

	const (
		outY = bottom - control.PortRadius - 1.0
		inY  = top + 2.75*mmPerHp
	)
	var (
		width = float64(hp) * mmPerHp
		right = width - left
	)
	p.VLine(stepBlockRight, channelSeparatorTop, bottom)
	p.HLine(stepBlockRight, right, levelY)
	p.LevelRangeSwitch(right, levelY, 2)
	p.HLine(stepBlockRight, right, durationY)
	p.DurationRangeSwitch(right, durationY, 2)
	p.InPort(right, inY, "IN")
	p.OutPort(right, outY, "OUT")

	return p
}
