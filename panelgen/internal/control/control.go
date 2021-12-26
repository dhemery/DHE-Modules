package control

import "dhemery.com/DHE-Modules/panelgen/internal/svg"

type Control struct {
	Frames       map[string]svg.Element
	DefaultFrame svg.Element
}

func (c Control) Top() float64 {
	return c.DefaultFrame.Top()
}
func (c Control) Right() float64 {
	return c.DefaultFrame.Right()
}
func (c Control) Bottom() float64 {
	return c.DefaultFrame.Bottom()
}
func (c Control) Left() float64 {
	return c.DefaultFrame.Left()
}
func (c Control) Width() float64 {
	return c.DefaultFrame.Width()
}
func (c Control) Height() float64 {
	return c.DefaultFrame.Height()
}
