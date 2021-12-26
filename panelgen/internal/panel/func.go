package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("func", Func)
	registerBuilder("func-6", Func6)
}

const (
	funcHue      = 220.0
	funcTop      = 23.0
	funcBottom   = 108.0
	funcRowCount = 6
	funcDy       = (funcBottom - funcTop) / float64(funcRowCount-1)
	funcDir      = "func"
)

var (
	funcFg                     = svg.HslColor(funcHue, 1, .4)
	funcBg                     = svg.HslColor(funcHue, .5, .96)
	funcOffsetRangeNames       = []string{"0–5", "±5", "0–10", "±10"}
	funcOffsetRangeStepper     = control.Stepper("offset-range", funcFg, funcBg, svg.SmallFont, 7, 2, funcOffsetRangeNames)
	funcMultiplierRangeNames   = []string{"0–1", "±1", "0–2", "±2", "0–5", "±5", "0–10", "±10"}
	funcMultiplierRangeStepper = control.Stepper("multiplier-range", funcFg, funcBg, svg.SmallFont, 7, 2, funcMultiplierRangeNames)
	funcKnob                   = control.LargeKnob(funcFg, funcBg)
)

func Func() *Panel {
	const (
		hp     = 3
		width  = mmPerHp * hp
		center = width / 2.0
	)
	p := NewPanel("FUNC", hp, funcFg, funcBg, funcDir)

	y := funcTop
	p.InPort(center, y+boxedPortYOffset, "IN")
	y += funcDy
	p.ThumbSwitch(center, y, 1, "ADD", "MULT")
	y += funcDy
	p.Install(center, y, funcKnob)
	y += funcDy
	p.Install(center, y, funcOffsetRangeStepper)
	p.Add(funcMultiplierRangeStepper)
	y += 2.0 * funcDy
	p.OutPort(center, y+boxedPortYOffset, "OUT")

	return p
}

func Func6() *Panel {
	const (
		hp = 12
	)
	p := NewPanel("FUNC 6", hp, funcFg, funcBg, funcDir)

	var (
		width       = mmPerHp * hp
		center      = width / 2.0
		left        = width / 7.0
		leftCenter  = (left + center) / 2.0
		right       = width - left
		rightCenter = width - leftCenter
	)
	for channel := 0; channel < funcRowCount; channel++ {
		y := funcTop + float64(channel)*funcDy
		p.HLine(left, right, y)
		p.InPort(left, y+boxedPortYOffset, "IN")
		p.ThumbSwitch(leftCenter, y, 1, "ADD", "MULT")
		p.Install(center, y, funcKnob)
		p.Install(rightCenter, y, funcOffsetRangeStepper)
		p.Add(funcMultiplierRangeStepper)
		p.OutPort(right, y+boxedPortYOffset, "OUT")
	}

	return p
}
