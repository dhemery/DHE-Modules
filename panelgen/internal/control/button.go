package control

import (
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

const (
	ButtonDiameter = 6.0
	ButtonRadius   = ButtonDiameter / 2.0
)

func Button(pressedColor, releasedColor svg.Color) Control {
	return button("button", pressedColor, releasedColor)
}

func OutputButton(pressedColor, releasedColor svg.Color) Control {
	return button("output-button", pressedColor, releasedColor)
}

func buttonState(buttonColor, ringColor svg.Color) svg.Circle {
	const (
		strokeWidth = 1.0
		radius      = ButtonRadius - strokeWidth/2
	)
	return svg.Circle{
		R:           radius,
		Stroke:      ringColor,
		StrokeWidth: strokeWidth,
		Fill:        buttonColor,
	}
}

func button(slug string, pressedColor, releasedColor svg.Color) Control {
	released := buttonState(releasedColor, releasedColor)
	pressed := buttonState(pressedColor, releasedColor)
	return Control{
		Frames: map[string]svg.Element{
			slug + "-released": released,
			slug + "-pressed":  pressed,
		},
		DefaultFrame: released,
	}
}
