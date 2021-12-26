package control

import "dhemery.com/DHE-Modules/panelgen/internal/svg"

func SelectionMarker(selection int, color svg.Color) Control {
	frames := []svg.Element{
		selectionMarker(-1, color),
		selectionMarker(1, color),
	}
	return Control{
		Frames: map[string]svg.Element{
			"marker-start": frames[0],
			"marker-end":   frames[1],
		},
		DefaultFrame: frames[selection],
	}

}

func selectionMarker(direction float64, color svg.Color) svg.Group {
	const (
		nibLength   = LightDiameter
		strokeWidth = 0.35
	)

	nibEnd := direction * nibLength / 2.0
	topNib := svg.Line{
		X2:          nibEnd,
		Y1:          -nibLength,
		Y2:          -nibLength,
		Stroke:      color,
		StrokeWidth: 0.35,
		Cap:         "round",
	}
	beam := svg.Line{
		X1:          nibEnd,
		X2:          nibEnd,
		Y1:          nibLength,
		Y2:          -nibLength,
		Stroke:      color,
		StrokeWidth: 0.35,
		Cap:         "round",
	}
	antiNib := svg.Line{
		X2:          -nibEnd,
		Stroke:      svg.NoColor,
		StrokeWidth: 0.35,
		Cap:         "round",
	}
	bottomNib := svg.Line{
		X2:          nibEnd,
		Y1:          nibLength,
		Y2:          nibLength,
		Stroke:      color,
		StrokeWidth: 0.35,
		Cap:         "round",
	}
	return svg.GroupOf(topNib, beam, antiNib, bottomNib)

}
