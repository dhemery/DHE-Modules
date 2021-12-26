package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	registerBuilder("buttons", buttons)
}

func buttons() *Panel {
	const (
		hue = 60
		hp  = 5
	)

	var (
		fg          = svg.HslColor(hue, 1.0, 0.15)
		bg          = svg.HslColor(hue, 1.0, 0.97)
		button      = control.Button(bg, fg)
		negateLabel = labelAbove("¬", button, svg.LargeFont, fg)
		port        = control.Port(fg, bg)
	)

	p := NewPanel("BUTTONS", hp, fg, bg, "buttons")

	top := 21.0
	dy := 12.7
	button_x := 5.0
	negate_x := button_x + 6.5
	port_x := negate_x + 7.7

	labels := []string{"A", "B", "C", "D", "E", "F", "G", "H"}
	for i, name := range labels {
		y := top + float64(i)*dy

		p.Install(button_x, y, button)
		p.Install(negate_x, y, button)
		p.Install(port_x, y, port)

		p.Engrave(button_x, y, labelAbove(name, button, svg.LargeFont, fg))
		p.Engrave(negate_x, y, negateLabel)
	}

	return p
}
