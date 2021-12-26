package panel

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("cubic", cubic)
}

func cubic() *Panel {
	const (
		hue = 180
		hp  = 5
	)
	var (
		bg = svg.HslColor(hue, 1, .97)
		fg = svg.HslColor(hue, 1, .3)
	)

	p := NewPanel("CUBIC", hp, fg, bg, "cubic")

	const (
		top    = 20.0
		deltaY = 15.0
		width  = mmPerHp * hp
		left   = width/4.0 + 1.0/3.0
		right  = width - left
	)

	for row := 0; row < 4; row++ {
		y := top + deltaY*float64(row)
		p.CvPort(left, y)
		p.SmallKnob(right, y, coefficientKnobLabel(3-row))
	}

	y := 82.0
	p.SmallKnob(left, y, "IN")
	p.SmallKnob(right, y, "OUT")

	y = y + deltaY
	p.CvPort(left, y)  // IN gain
	p.CvPort(right, y) // OUT gain

	y = y + deltaY
	p.InPort(left, y, "IN")
	p.OutPort(right, y, "OUT")

	return p
}

func coefficientKnobLabel(exponent int) string {
	const labelFormat = `X<tspan baseline-shift="super">%d</tspan>`
	return fmt.Sprintf(labelFormat, exponent)
}
