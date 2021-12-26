package control

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func Stepper(stepperSlug string, stroke, fill svg.Color, font svg.Font, width float64, selection int, stateLabels []string) Control {
	const (
		padding      = 1
		strokeWidth  = 0.25
		cornerRadius = 2.0 * strokeWidth
	)
	var defaultFrame svg.Element
	frames := map[string]svg.Element{}

	for i, stateLabel := range stateLabels {
		frameSlug := fmt.Sprintf("%s-%d", stepperSlug, i+1)
		label := svg.TextCentered(stateLabel, font, stroke)
		height := label.Height() + 2.0*padding
		box := svg.Rect{
			X:           -width / 2.0,
			Y:           -height / 2.0,
			H:           height,
			W:           width,
			StrokeWidth: strokeWidth,
			Stroke:      stroke,
			Fill:        fill,
			RX:          cornerRadius,
			RY:          cornerRadius,
		}
		frame := svg.GroupOf(box, label)
		if selection == i+1 {
			defaultFrame = frame
		}
		frames[frameSlug] = frame
	}

	return Control{
		Frames:       frames,
		DefaultFrame: defaultFrame,
	}
}
