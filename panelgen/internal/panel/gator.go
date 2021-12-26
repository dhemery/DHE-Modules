package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("gator", gator)
}

func gator() *Panel {
	const (
		hue = 220.0
		hp  = 9
	)
	var (
		fg = svg.HslColor(hue, 1, .3)
		bg = svg.HslColor(hue, 1, .97)
	)
	p := NewPanel("GATOR", hp, fg, bg, "gator")

	const (
		top         = 3.5 * mmPerHp
		dy          = 1.75 * mmPerHp
		left        = 1.5 * mmPerHp
		leftCenter  = 3.25 * mmPerHp
		width       = float64(hp) * mmPerHp
		rightCenter = width - leftCenter
		right       = width - left
	)
	var (
		port   = control.Port(fg, bg)
		button = control.Button(bg, fg)
	)
	for row := 0; row < 8; row++ {
		y := top + float64(row)*dy
		p.Install(left, y, port)
		p.Install(leftCenter, y, button)
		p.Install(rightCenter, y, button)
		p.Install(right, y, port)
	}

	y := top + 8.75*dy
	p.OutPort(left, y, "AND")
	p.OutPort(leftCenter, y, "NAND")
	p.OutPort(rightCenter, y, "OR")
	p.OutPort(right, y, "NOR")

	y += 2.75 * mmPerHp
	p.OutPort(left, y, "EVEN")
	p.OutPort(leftCenter, y, "ODD")
	p.OutPort(rightCenter, y, "XOR")
	p.OutPort(right, y, "XNOR")

	return p
}
