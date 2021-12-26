package svg

type Rect struct {
	XMLName     string  `xml:"rect"`
	X           float64 `xml:"x,attr,omitempty"`
	Y           float64 `xml:"y,attr,omitempty"`
	W           float64 `xml:"width,attr,omitempty"`
	H           float64 `xml:"height,attr,omitempty"`
	RX          float64 `xml:"rx,attr,omitempty"`
	RY          float64 `xml:"ry,attr,omitempty"`
	Fill        Color   `xml:"fill,attr"`
	Stroke      Color   `xml:"stroke,attr"`
	StrokeWidth float64 `xml:"stroke-width,attr,omitempty"`
}

func (r Rect) Top() float64 {
	return r.Y - r.StrokeWidth/2
}

func (r Rect) Right() float64 {
	return r.X + r.W + r.StrokeWidth/2
}

func (r Rect) Bottom() float64 {
	return r.Y + r.H + r.StrokeWidth/2
}

func (r Rect) Left() float64 {
	return r.X - r.StrokeWidth/2
}

func (r Rect) Width() float64 {
	return r.Right() - r.Left()
}

func (r Rect) Height() float64 {
	return r.Bottom() - r.Top()
}

func (r Rect) Translate(dx, dy float64) Element {
	r.X += dx
	r.Y += dy
	return r
}
