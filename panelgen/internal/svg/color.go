package svg

import (
	"fmt"
	"math"
)

type Color string

var (
	NoColor = Color("none")
)

func HslColor(hue float64, saturation float64, luminance float64) Color {
	twoL := 2 * luminance
	chroma := (1 - math.Abs(twoL-1)) * saturation
	hp := hue / 60
	hpMod2 := math.Mod(hp, 2)
	x := chroma * (1 - math.Abs(hpMod2-1))

	var rp, gp, bp float64

	switch int64(hp) {
	case 0:
		rp, gp, bp = chroma, x, 0
	case 1:
		rp, gp, bp = x, chroma, 0
	case 2:
		rp, gp, bp = 0, chroma, x
	case 3:
		rp, gp, bp = 0, x, chroma
	case 4:
		rp, gp, bp = x, 0, chroma
	case 5:
		rp, gp, bp = chroma, 0, x
	default:
		panic(fmt.Sprintf("HSL %f,%f,%f out of range", hue, saturation, luminance))
	}
	m := luminance - chroma/2
	s := fmt.Sprintf("#%02x%02x%02x", uint8((rp+m)*255+0.5), uint8((gp+m)*255+0.5), uint8((bp+m)*255+0.5))
	return Color(s)
}
