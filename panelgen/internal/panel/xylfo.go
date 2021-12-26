package panel

import "dhemery.com/DHE-Modules/panelgen/internal/svg"

func init() {
	blossom := xyLfoLayout{
		name: "BLOSSOM",
	}
	xycloid := xyLfoLayout{
		name:              "XYCLOID",
		depthSwitchLabels: []string{"IN", "", "OUT"},
	}

	registerBuilder("blossom", blossom.Build)
	registerBuilder("xycloid", xycloid.Build)
}

type xyLfoLayout struct {
	name              string
	depthSwitchLabels []string
}

func (l xyLfoLayout) Build() *Panel {
	const (
		hue = 270
		hp  = 10
	)

	var (
		fg = svg.HslColor(hue, 1.0, 0.5)
		bg = svg.HslColor(hue, 0.66, 0.97)
	)
	p := NewPanel(l.name, hp, fg, bg, "xycloid")

	const (
		width       = hp * mmPerHp
		left        = width / 7.0
		right       = width - left
		leftCenter  = (right-left)/3.0 + left
		rightCenter = width - leftCenter

		inputTop  = 25.0
		inputDy   = 18.5
		outputTop = 97.0
		outputDy  = 15.0
	)

	y := inputTop
	p.HLine(left, rightCenter, y)
	p.CvPort(left, y)
	p.Attenuverter(leftCenter, y)
	p.LargeKnob(rightCenter, y, "SPEED")

	y += inputDy
	p.HLine(left, right, y)
	p.CvPort(left, y)
	p.Attenuverter(leftCenter, y)
	p.LargeKnob(rightCenter, y, "RATIO")
	p.ThumbSwitch(right, y, 2, "LOCK", "FREE")

	y += inputDy
	p.CvPort(left, y)
	p.Attenuverter(leftCenter, y)
	p.LargeKnob(rightCenter, y, "DEPTH")
	if len(l.depthSwitchLabels) > 0 {
		p.HLine(left, right, y)
		p.ThumbSwitch(right, y, 2, l.depthSwitchLabels...)
	} else {
		p.HLine(left, rightCenter, y)

	}

	y += inputDy
	p.HLine(left, rightCenter, y)
	p.CvPort(left, y)
	p.Attenuverter(leftCenter, y)
	p.LargeKnob(rightCenter, y, "PHASE")

	y = outputTop
	p.HLine(left, right, y)
	p.CvPort(left, y)
	p.SmallKnob(leftCenter, y, "GAIN")
	p.LevelRangeSwitch(rightCenter, y, 1)
	p.OutPort(right, y+boxedPortYOffset, "X OUT")

	y += outputDy
	p.HLine(left, right, y)
	p.CvPort(left, y)
	p.SmallKnob(leftCenter, y, "GAIN")
	p.LevelRangeSwitch(rightCenter, y, 1)
	p.OutPort(right, y+boxedPortYOffset, "Y OUT")

	return p
}
