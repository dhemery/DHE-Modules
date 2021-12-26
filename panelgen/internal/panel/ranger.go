package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("ranger", ranger)
}

func ranger() *Panel {
	const (
		hue = 60
		hp  = 6
	)
	var (
		fg = svg.HslColor(hue, 1, .15)
		bg = svg.HslColor(hue, 1, .97)
	)
	p := NewPanel("RANGER", hp, fg, bg, "ranger")

	const (
		width  = float64(hp) * mmPerHp
		left   = 1.5 * mmPerHp
		center = width / 2.0
		right  = width - left
	)

	y := 14.5
	dy := 15.0
	p.HSeparator(y)

	y += dy * 0.75
	p.MediumKnob(left, y, "LEVEL")
	p.OutPort(right, y, "OUT")

	y += dy
	p.CvPort(left, y)
	p.Attenuverter(right, y)

	y += dy / 2.0
	limitTop := y
	p.HSeparator(y)

	y += dy * 0.75
	p.MediumKnob(left, y, "CCW")
	p.MediumKnob(right, y, "CW")

	y += dy
	p.Attenuverter(left, y)
	p.Attenuverter(right, y)

	y += dy
	p.CvPort(left, y)
	p.CvPort(right, y)

	y += dy
	p.LevelRangeSwitch(left, y, 1)
	p.LevelRangeSwitch(right, y, 1)

	y += dy * 0.6
	limitBottom := y
	p.HSeparator(y)
	p.VLine(center, limitTop, limitBottom)

	return p
}
