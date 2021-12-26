package control

import (
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func TinyKnob(knobColor, pointerColor svg.Color) Control {
	return knob("knob-tiny", TinyKnobDiameter, knobColor, pointerColor)
}

func SmallKnob(knobColor, pointerColor svg.Color) Control {
	return knob("knob-small", SmallKnobDiameter, knobColor, pointerColor)
}

func MediumKnob(knobColor, pointerColor svg.Color) Control {
	return knob("knob-medium", MediumKnobDiameter, knobColor, pointerColor)
}

func LargeKnob(knobColor, pointerColor svg.Color) Control {
	return knob("knob-large", LargeKnobDiameter, knobColor, pointerColor)
}

const (
	HugeKnobDiameter   = 19
	LargeKnobDiameter  = 12.7
	MediumKnobDiameter = 10
	SmallKnobDiameter  = 8.4
	SmallKnobRadius    = SmallKnobDiameter / 2.0
	TinyKnobDiameter   = 7
	TinyKnobRadius     = TinyKnobDiameter / 2.0
)

func knob(slug string, diameter float64, knobColor, pointerColor svg.Color) Control {
	radius := diameter / 2
	knob := svg.Circle{
		R:    radius,
		Fill: knobColor,
	}

	pointerWidth := radius / 8
	pointerLength := radius - pointerWidth
	pointer := svg.Line{
		Stroke:      pointerColor,
		StrokeWidth: pointerWidth,
		Y2:          -pointerLength,
		Cap:         "round",
	}
	frame := svg.GroupOf(knob, pointer)
	return Control{
		Frames:       map[string]svg.Element{slug: frame},
		DefaultFrame: frame,
	}
}
