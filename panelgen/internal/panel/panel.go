package panel

import (
	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

const (
	mmPerHp     = 5.08
	strokeWidth = 0.35
)

type Hp int

func (hp Hp) toMM() float64 {
	return float64(hp) * mmPerHp
}

type Panel struct {
	FrameDir   string
	Engravings []svg.Bounded
	Frames     []svg.Bounded
	Controls   []control.Control
	Fg, Bg     svg.Color
	Width      float64
}

const (
	padding            = 1.0
	buttonPortDistance = control.PortRadius + control.ButtonRadius + padding
	boxedPortYOffset   = 1.25
	outlineThickness   = 0.5
	panelHeight        = 128.5
)

func NewPanel(name string, hp Hp, fg, bg svg.Color, dir string) *Panel {
	const (
		nameLabelY  = 9.0
		brandLabelY = panelHeight - nameLabelY
	)

	p := &Panel{
		FrameDir: dir,
		Fg:       fg,
		Bg:       bg,
		Width:    hp.toMM(),
	}

	width := float64(hp) * mmPerHp
	faceplateRect := svg.Rect{
		X:           outlineThickness / 2,
		Y:           outlineThickness / 2,
		W:           width - outlineThickness,
		H:           panelHeight - outlineThickness,
		Fill:        bg,
		Stroke:      fg,
		StrokeWidth: outlineThickness,
	}
	p.Engrave(0, 0, faceplateRect)
	center := width / 2

	p.Engrave(center, nameLabelY, svg.TextAbove(name, svg.TitleFont, p.Fg))
	p.Engrave(center, brandLabelY, svg.TextBelow("DHE", svg.TitleFont, p.Fg))
	return p
}

func (p *Panel) Port(x, y float64, name string, labelColor svg.Color) {
	port := control.Port(p.Fg, p.Bg)
	p.Install(x, y, port)
	p.Engrave(x, y, labelAbove(name, port, svg.SmallFont, p.Fg))
}

func (p *Panel) CvPort(x, y float64) {
	p.Port(x, y, "CV", p.Fg)
}

func (p *Panel) Line(x1, y1, x2, y2 float64) {
	line := svg.Line{
		X1:          x1,
		Y1:          y1,
		X2:          x2,
		Y2:          y2,
		Stroke:      p.Fg,
		StrokeWidth: strokeWidth,
	}
	p.Engrave(0, 0, line)
}

func (p *Panel) HLine(x1, x2, y float64) {
	p.Line(x1, y, x2, y)
}

func (p *Panel) HSeparator(y float64) {
	left := outlineThickness / 2.0
	right := p.Width - left
	p.HLine(left, right, y)
}

func (p *Panel) VLine(x, y1, y2 float64) {
	p.Line(x, y1, x, y2)
}

func (p *Panel) Button(x, y float64, name string) {
	button := control.Button(p.Bg, p.Fg)
	label := labelAbove(name, button, svg.SmallFont, p.Fg)

	p.Install(x, y, button)
	p.Engrave(x, y, label)
}

func (p *Panel) InPort(x, y float64, name string) {
	port := control.Port(p.Fg, p.Bg)
	label := labelAbove(name, port, svg.SmallFont, p.Fg)
	box := boxAround(p.Fg, p.Bg, port, label)
	p.Install(x, y, port)
	p.Engrave(x, y, box)
	p.Engrave(x, y, label)
}

func (p *Panel) InButtonPort(x, y float64, name string) {
	port := control.Port(p.Fg, p.Bg)
	button := control.Button(p.Bg, p.Fg)
	p.Install(x, y, port)
	p.Install(x+buttonPortDistance, y, button)

	label := labelAbove(name, port, svg.SmallFont, p.Fg)
	box := boxAround(p.Fg, p.Bg, port, button.DefaultFrame.Translate(buttonPortDistance, 0), label)
	p.Engrave(x, y, box)
	p.Engrave(x, y, label)
}

func (p *Panel) OutPort(x, y float64, name string) {
	port := control.Port(p.Fg, p.Bg)
	p.Install(x, y, port)

	label := labelAbove(name, port, svg.SmallFont, p.Bg)
	p.Engrave(x, y, boxAround(p.Fg, p.Fg, port, label))
	p.Engrave(x, y, label)
}

func (p *Panel) OutButtonPort(x, y float64, name string) {
	port := control.Port(p.Fg, p.Bg)
	button := control.OutputButton(p.Fg, p.Bg)
	p.Install(x, y, port)
	p.Install(x-buttonPortDistance, y, button)

	label := labelAbove(name, port, svg.SmallFont, p.Bg)
	p.Engrave(x, y, boxAround(p.Fg, p.Fg, port, button.DefaultFrame.Translate(-buttonPortDistance, 0), label))
	p.Engrave(x, y, label)
}

func (p *Panel) SmallKnob(x, y float64, name string) {
	knob := control.SmallKnob(p.Fg, p.Bg)
	p.Install(x, y, knob)
	p.Engrave(x, y, labelAbove(name, knob, svg.SmallFont, p.Fg))
}

func (p *Panel) MediumKnob(x, y float64, name string) {
	knob := control.MediumKnob(p.Fg, p.Bg)
	p.Install(x, y, knob)
	p.Engrave(x, y, labelAbove(name, knob, svg.SmallFont, p.Fg))
}

func (p *Panel) LargeKnob(x, y float64, name string) {
	knob := control.LargeKnob(p.Fg, p.Bg)
	p.Install(x, y, knob)
	p.Engrave(x, y, labelAbove(name, knob, svg.LargeFont, p.Fg))
}

func (p *Panel) Attenuverter(x, y float64) {
	knob := control.TinyKnob(p.Fg, p.Bg)
	p.Install(x, y, knob)
	p.Engrave(x, y, labelAbove("- +", knob, svg.LargeFont, p.Fg))
}

func (p *Panel) ThumbSwitch(x, y float64, selection int, labels ...string) {
	size := len(labels)
	thumbSwitch := control.ThumbSwitch(size, selection, p.Fg, p.Bg)
	p.Install(x, y, thumbSwitch)
	p.Engrave(x, y, labelBelow(labels[0], thumbSwitch, svg.SmallFont, p.Fg))
	p.Engrave(x, y, labelAbove(labels[size-1], thumbSwitch, svg.SmallFont, p.Fg))
	if size == 3 {
		p.Engrave(x, y, labelRight(labels[1], thumbSwitch, svg.SmallFont, p.Fg))
	}
}

func (p *Panel) DurationRangeSwitch(x, y float64, selection int) {
	p.ThumbSwitch(x, y, selection, "1", "10", "100")
}
func (p *Panel) LevelRangeSwitch(x, y float64, selection int) {
	p.ThumbSwitch(x, y, selection, "BI", "UNI")
}
func (p *Panel) ShapeSwitch(x, y float64, selection int) {
	p.ThumbSwitch(x, y, selection, "J", "S")
}

func (p *Panel) Stepper(x, y float64, name string, width float64, selection int, labels ...string) {
	stepper := control.Stepper("ratio-mode", p.Fg, p.Bg, svg.SmallFont, width, selection, labels)
	label := labelAbove(name, stepper, svg.SmallFont, p.Fg)

	p.Install(x, y, stepper)
	p.Engrave(x, y, label)
}

func (p *Panel) Light(x, y float64) {
	p.Engrave(x, y, svg.Circle{
		R:           control.LightRadius,
		Fill:        p.Fg,
		Stroke:      p.Fg,
		StrokeWidth: strokeWidth,
	})
}

// Add adds the control to the panel's list of controls.
func (p *Panel) Add(c control.Control) {
	p.Controls = append(p.Controls, c)
}

// Install adds the control and draws its default frame at the specified position.
func (p *Panel) Install(x, y float64, c control.Control) {
	p.Add(c)
	p.Frames = append(p.Frames, c.DefaultFrame.Translate(x, y))
}

// Engrave engraves the shape into the faceplate.
func (p *Panel) Engrave(x, y float64, e svg.Element) {
	p.Engravings = append(p.Engravings, e.Translate(x, y))
}

func (p *Panel) ImageSvg() svg.Svg {
	faceplateGroup := svg.GroupOf(p.Engravings...)
	faceplateGroup.Id = "faceplate"
	controlsGroup := svg.GroupOf(p.Frames...)
	controlsGroup.Id = "controls"
	return svg.NewSvg(faceplateGroup, controlsGroup)
}

func (p *Panel) FrameSvgs() map[string]svg.Svg {
	frames := map[string]svg.Svg{}
	for _, control := range p.Controls {
		for slug, frame := range control.Frames {
			frames[slug] = svg.NewSvg(frame)
		}
	}
	return frames
}

func boxAround(stroke, fill svg.Color, elements ...svg.Bounded) svg.Rect {
	const (
		cornerRadius = 1.0
		strokeWidth  = 0.35
	)
	bounds := svg.Bounds(elements...)
	return svg.Rect{
		X:           bounds.Left() - padding,
		Y:           bounds.Top() - padding,
		H:           bounds.Height() + 2.0*padding,
		W:           bounds.Width() + 2.0*padding,
		Fill:        fill,
		Stroke:      stroke,
		StrokeWidth: strokeWidth,
		RX:          cornerRadius,
		RY:          cornerRadius,
	}
}

func labelAbove(text string, base svg.Bounded, font svg.Font, color svg.Color) svg.Element {
	return svg.TextAbove(text, font, color).Translate(0, base.Top()-padding)
}
func labelBelow(text string, base svg.Bounded, font svg.Font, color svg.Color) svg.Element {
	return svg.TextBelow(text, font, color).Translate(0, base.Bottom()+padding)
}
func labelRight(text string, base svg.Bounded, font svg.Font, color svg.Color) svg.Element {
	return svg.TextRight(text, font, color).Translate(base.Right()+padding, 0)
}
