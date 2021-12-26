package control

import (
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func Port(fg, bg svg.Color) Control {
	const (
		slug         = "port"
		fgThickness  = .45
		bgThickness  = .95
		nutRadius    = PortRadius
		barrelRadius = 3.5
		holeRadius   = 2.325
	)
	nut := svg.Circle{
		R:           nutRadius,
		Stroke:      svg.NoColor,
		StrokeWidth: 0,
		Fill:        fg,
	}
	barrel := svg.Circle{
		R:           barrelRadius,
		Stroke:      bg,
		StrokeWidth: bgThickness,
		Fill:        svg.NoColor,
	}
	hole := svg.Circle{
		R:           holeRadius,
		Stroke:      bg,
		StrokeWidth: bgThickness,
		Fill:        svg.NoColor,
	}
	frame := svg.GroupOf(nut, barrel, hole)
	return Control{
		Frames:       map[string]svg.Element{slug: frame},
		DefaultFrame: frame,
	}
}

const (
	PortDiameter = 8.4
	PortRadius   = PortDiameter / 2
)
