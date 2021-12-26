package control

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

const (
	ThumbSwitchWidth = 3.0
)

func thumbSwitchSpec(size int, knurlThickness float64) (yOffset, deltaY float64) {
	if size == 2 {
		offset := ThumbSwitchWidth/2 - knurlThickness
		return offset, 2 * offset
	}
	offset := ThumbSwitchWidth - knurlThickness
	return offset, offset
}

func ThumbSwitch(size, selection int, stroke, fill svg.Color) Control {
	const (
		housingThickness = ThumbSwitchWidth / 8.0
		housingWidth     = ThumbSwitchWidth - housingThickness
		cornerRadius     = housingThickness / 2.0
		knurlSpacing     = 0.5
		knurlThickness   = knurlSpacing / 2.0
		knurlLength      = housingWidth - 2*knurlSpacing
	)
	lever := thumbSwitchLever(knurlLength, knurlThickness, stroke, fill)
	levers := []svg.Bounded{}
	leverY, leverDeltaY := thumbSwitchSpec(size, knurlThickness)
	for i := 0; i < size; i++ {
		levers = append(levers, lever.Translate(0, leverY))
		leverY -= leverDeltaY
	}
	b := svg.Bounds(levers...)
	boxWidth := b.Width() + 2*housingThickness
	boxHeight := b.Height() + 2*housingThickness + .125

	housing := svg.Rect{
		X:           -boxWidth / 2,
		Y:           -boxHeight / 2,
		W:           boxWidth,
		H:           boxHeight,
		Stroke:      stroke,
		StrokeWidth: housingThickness,
		Fill:        fill,
		RX:          cornerRadius,
		RY:          cornerRadius,
	}
	var defaultFrame svg.Element
	frames := map[string]svg.Element{}
	for i := 0; i < size; i++ {
		slug := fmt.Sprint("thumb-switch-", size, "-", i+1)
		frame := svg.GroupOf(housing, levers[i])
		frames[slug] = frame
		if selection == i+1 {
			defaultFrame = frame
		}
	}

	return Control{
		Frames:       frames,
		DefaultFrame: defaultFrame,
	}
}

func thumbSwitchKnurl(length, thickness float64, stroke svg.Color) svg.Line {
	return svg.Line{
		X1:          -length / 2,
		X2:          length / 2,
		StrokeWidth: thickness,
		Stroke:      stroke,
		Cap:         "round",
	}
}

func thumbSwitchLever(knurlLength, knurlThickness float64, stroke, fill svg.Color) svg.Group {
	knurl := thumbSwitchKnurl(knurlLength, knurlThickness, stroke)
	knurls := []svg.Bounded{}
	for i := -2; i <= 2; i++ {
		yOffset := knurlThickness * 2 * float64(i)
		knurls = append(knurls, knurl.Translate(0, yOffset))
	}
	return svg.GroupOf(knurls...)
}
