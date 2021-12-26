package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("booster-stage", boosterStage)
	registerBuilder("hostage", hostage)
	registerBuilder("stage", stage)
	registerBuilder("upstage", upstage)
}

const (
	stageParamsTop = 25.0
	stageParamsDy  = 18.5
	stagePortsTop  = 82.0
	stagePortsDy   = 15.0
)

func boosterStage() *Panel {
	const (
		hue = 0
		hp  = 8
	)

	var (
		bg = svg.HslColor(hue, 1.0, .97)
		fg = svg.HslColor(hue, 1.0, .3)
	)

	p := NewPanel("BOOSTER STAGE", hp, fg, bg, "booster-stage")

	const (
		width  = hp * mmPerHp
		left   = width/6.0 + 1.0/3.0
		right  = width - left
		center = width / 2.0
	)

	y := stageParamsTop
	p.HLine(left, right, y)
	p.CvPort(left, y)
	p.LargeKnob(center, y, "LEVEL")
	p.LevelRangeSwitch(right, y, 1)

	y += stageParamsDy
	p.HLine(left, right, y)
	p.CvPort(left, y)
	p.LargeKnob(center, y, "CURVE")
	p.ShapeSwitch(right, y, 1)

	y += stageParamsDy
	p.HLine(left, right, y)
	p.CvPort(left, y)
	p.LargeKnob(center, y, "DURATION")
	p.DurationRangeSwitch(right, y, 2)

	y = stagePortsTop
	p.InButtonPort(left, y, "DEFER")
	p.OutButtonPort(right, y, "ACTIVE")

	y += stagePortsDy
	p.InButtonPort(left, y, "TRIG")
	p.OutButtonPort(right, y, "EOC")

	y += stagePortsDy
	p.InPort(left, y, "IN")
	p.OutPort(right, y, "OUT")
	return p
}

func hostage() *Panel {
	const (
		hue = 300.0
		hp  = 5
	)
	var (
		fg = svg.HslColor(hue, 1, .3)
		bg = svg.HslColor(hue, 1, .97)
	)
	p := NewPanel("HOSTAGE", hp, fg, bg, "hostage")

	const (
		width  = hp * mmPerHp
		left   = width/4.0 + 1.0/3.0
		right  = width - left
		center = width / 2.0
	)

	y := stageParamsTop
	p.ThumbSwitch(center, y, 1, "HOLD", "SUSTAIN")

	y += stageParamsDy
	p.CvPort(left, y)
	p.DurationRangeSwitch(right, y, 2)

	y += stageParamsDy
	p.LargeKnob(center, y, "DURATION")

	y = stagePortsTop
	p.InPort(left, y, "DEFER")
	p.OutPort(right, y, "ACTIVE")

	y += stagePortsDy
	p.InPort(left, y, "GATE")
	p.OutPort(right, y, "EOC")

	y += stagePortsDy
	p.InPort(left, y, "IN")
	p.OutPort(right, y, "OUT")

	return p
}

func stage() *Panel {
	const (
		hue = 120
		hp  = 5
	)

	var (
		bg = svg.HslColor(hue, 1.0, .97)
		fg = svg.HslColor(hue, 1.0, .3)
	)

	p := NewPanel("STAGE", hp, fg, bg, "stage")

	const (
		width  = hp * mmPerHp
		left   = width/4.0 + 1.0/3.0
		right  = width - left
		center = width / 2.0
	)

	y := stageParamsTop
	p.LargeKnob(center, y, "LEVEL")

	y += stageParamsDy
	p.LargeKnob(center, y, "CURVE")

	y += stageParamsDy
	p.LargeKnob(center, y, "DURATION")

	y = stagePortsTop
	p.InPort(left, y, "DEFER")
	p.OutPort(right, y, "ACTIVE")

	y += stagePortsDy
	p.InPort(left, y, "TRIG")
	p.OutPort(right, y, "EOC")

	y += stagePortsDy
	p.InPort(left, y, "IN")
	p.OutPort(right, y, "OUT")
	return p
}

func upstage() *Panel {
	const (
		hue = 210
		hp  = 5
	)

	var (
		bg = svg.HslColor(hue, 1.0, .97)
		fg = svg.HslColor(hue, 1.0, .3)
	)

	p := NewPanel("UPSTAGE", hp, fg, bg, "upstage")

	const (
		width  = hp * mmPerHp
		left   = width/4.0 + 1.0/3.0
		right  = width - left
		center = width / 2.0
	)

	y := stageParamsTop
	p.LargeKnob(center, y, "LEVEL")

	y += stageParamsDy
	p.CvPort(left, y)
	p.LevelRangeSwitch(right, y, 1)

	y += stageParamsDy
	p.Button(left, y, "WAIT")
	p.Button(right, y, "TRIG")

	y = stagePortsTop
	p.InPort(left, y, "WAIT")

	y += stagePortsDy
	p.InPort(left, y, "TRIG")
	p.OutPort(right, y, "TRIG")

	y += stagePortsDy
	p.OutPort(right, y, "OUT")
	return p
}
