package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("swave", swave)
}

func swave() *Panel {
	const (
		hue = 16
		hp  = 4
	)
	var (
		fg = svg.HslColor(hue, 1, .5)
		bg = svg.HslColor(hue, 1, .97)
	)

	p := NewPanel("SWAVE", hp, fg, bg, "swave")

	const (
		width     = mmPerHp * hp
		x         = width / 2.0
		shaperTop = 25.0
		shaperDy  = 18.5
		portsTop  = 90.0
		portsDy   = 15.0
	)

	y := shaperTop
	shapeX := width / 4.0
	levelX := width - shapeX
	p.ShapeSwitch(shapeX, y, 1)
	p.LevelRangeSwitch(levelX, y, 1)
	y += shaperDy
	p.LargeKnob(x, y, "CURVE")
	y += shaperDy
	cvAvPadding := (width - control.PortDiameter - control.TinyKnobDiameter) / 3.0
	cvX := cvAvPadding + control.PortRadius
	avX := width - cvAvPadding - control.TinyKnobRadius
	p.CvPort(cvX, y)
	p.Attenuverter(avX, y)

	y = portsTop
	p.InPort(x, y, "IN")
	y += portsDy
	p.OutPort(x, y, "OUT")

	return p
}
