package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("tapers", tapers)
}

func tapers() *Panel {
	const (
		hue = 30
		hp  = 9
	)
	var (
		fg = svg.HslColor(hue, 1, .3)
		bg = svg.HslColor(hue, 1, .97)
	)
	p := NewPanel("TAPERS", hp, fg, bg, "tapers")

	const (
		width  = float64(hp) * mmPerHp
		left   = width/5.0 + 1.0/3.0
		right  = width - left
		center = width / 2.0

		top           = 24.0
		dy            = 16.0
		sectionBuffer = 4.0
	)

	addTapers := func(top float64) {
		y := top
		p.HLine(left, right, y)
		p.CvPort(left, y)
		p.Attenuverter(center, y)
		p.MediumKnob(right, y, "LEVEL")

		y += dy
		p.HLine(left, right, y)
		p.CvPort(left, y)
		p.Attenuverter(center, y)
		p.MediumKnob(right, y, "CURVE")

		y += dy
		p.ShapeSwitch(left, y, 1)
		p.LevelRangeSwitch(center, y, 1)
		p.OutPort(right, y, "OUT")
	}
	addTapers(top)
	p.HSeparator(panelHeight / 2.0)
	addTapers(top + 3.0*dy + sectionBuffer)
	return p
}
