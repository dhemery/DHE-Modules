package svg

import (
	"fmt"
)

type Svg struct {
	XMLName   string `xml:"svg"`
	Version   string `xml:"version,attr,omitempty"`
	Namespace string `xml:"xmlns,attr,omitempty"`
	ViewBox   string `xml:"viewBox,attr,omitempty"`
	Height    string `xml:"height,attr,omitempty"`
	Width     string `xml:"width,attr,omitempty"`
	Content   []Bounded
}

func NewSvg(content ...Bounded) Svg {
	b := Bounds(content...)
	s := Svg{
		Version:   "1.1",
		Namespace: "http://www.w3.org/2000/svg",
		ViewBox:   fmt.Sprintf("%f %f %f %f", b.Left(), b.Top(), b.Width(), b.Height()),
		Height:    fmt.Sprintf("%fmm", b.Height()),
		Width:     fmt.Sprintf("%fmm", b.Width()),
		Content:   content,
	}
	return s
}
