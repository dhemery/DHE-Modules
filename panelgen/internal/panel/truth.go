package panel

import (
	"fmt"

	"dhemery.com/DHE-Modules/panelgen/internal/control"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func init() {
	truths := []truth{
		truth2(),
		truth3(),
		truth4(),
	}
	for _, t := range truths {
		registerBuilder(t.slug(), t.build)
	}
}

type truth struct {
	hp         Hp
	inputNames []string
	conditionY float64
	outcomeX   float64
	inputTop   float64
	inputX     float64
	outputTop  float64
	outputX    float64
	portDy     float64
}

func truth2() truth {
	hp := Hp(8)
	centerX := hp.toMM() / 2.0
	offset := 3.5
	inputTop := 58.0
	portDy := 15.5
	return truth{
		hp:         hp,
		inputNames: []string{"A", "B"},
		conditionY: 21.0,
		outcomeX:   Hp(6).toMM(),
		inputTop:   inputTop,
		inputX:     centerX - offset,
		outputTop:  inputTop + 2.0*portDy,
		outputX:    centerX + offset,
		portDy:     portDy,
	}
}

func truth3() truth {
	hp := Hp(10)
	conditionY := 20.0
	outcomeX := Hp(8).toMM()
	portDy := 15.5
	inputTop := 76.0
	inputX := Hp(2).toMM()
	outputTop := inputTop + portDy
	outputX := outcomeX
	return truth{
		hp:         hp,
		inputNames: []string{"A", "B", "C"},
		conditionY: conditionY,
		outcomeX:   outcomeX,
		inputTop:   inputTop,
		inputX:     inputX,
		outputTop:  outputTop,
		outputX:    outputX,
		portDy:     portDy,
	}
}

func truth4() truth {
	hp := Hp(16)
	conditionY := 23.66
	outcomeX := 71.11
	portDy := 14.585
	inputTop := 29.36
	inputX := Hp(2).toMM()
	outputTop := inputTop + 4.0*portDy
	outputX := inputX + 7.0
	return truth{
		hp:         hp,
		inputNames: []string{"A", "B", "C", "D"},
		conditionY: conditionY,
		outcomeX:   outcomeX,
		inputTop:   inputTop,
		inputX:     inputX,
		outputTop:  outputTop,
		outputX:    outputX,
		portDy:     portDy,
	}
}

func (t truth) slug() string { return fmt.Sprintf("truth-%d", len(t.inputNames)) }

func (t truth) name() string { return fmt.Sprintf("TRUTH %d", len(t.inputNames)) }

func (t truth) build() *Panel {
	const (
		hue = 240
	)
	var (
		bg = svg.HslColor(hue, 1, .97)
		fg = svg.HslColor(hue, 1, .3)
	)

	p := NewPanel(t.name(), t.hp, fg, bg, "truth")

	nInputs := len(t.inputNames)
	for i := 0; i < nInputs-1; i++ {
		truthInButtonPort(p, t.inputX, t.inputTop+float64(i)*t.portDy, t.inputNames[i])
	}
	gateInputName := fmt.Sprintf("    %s/GATE", t.inputNames[nInputs-1])
	truthInButtonPort(p, t.inputX, t.inputTop+float64(nInputs-1)*t.portDy, gateInputName)

	var (
		conditionDx        = Hp(2).toMM()
		outcomeDy          = Hp(1).toMM()
		nConditions        = 1 << nInputs
		conditionX         = t.outcomeX - float64(nInputs)*conditionDx
		conditionBoxTop    = t.conditionY + 0.5*outcomeDy
		conditionBoxLeft   = conditionX - 0.5*conditionDx
		conditionBoxHeight = float64(nConditions) * outcomeDy
		conditionBoxWidth  = float64(nInputs) * conditionDx
		conditionBoxRight  = conditionBoxLeft + conditionBoxWidth
		conditionBoxBottom = conditionBoxTop + conditionBoxHeight
		conditionBox       = svg.Rect{
			W:           conditionBoxWidth,
			H:           conditionBoxHeight,
			Stroke:      fg,
			StrokeWidth: strokeWidth,
			Fill:        bg,
		}
	)
	p.Engrave(conditionBoxLeft, conditionBoxTop, conditionBox)
	for i := 1; i < nInputs; i++ {
		x := conditionBoxLeft + float64(i)*conditionDx
		p.VLine(x, conditionBoxTop, conditionBoxBottom)
	}
	for i := 1; i < nConditions; i++ {
		y := conditionBoxTop + float64(i)*outcomeDy
		p.HLine(conditionBoxLeft, conditionBoxRight, y)
	}

	var (
		headerBox = svg.Rect{
			W:           conditionBoxWidth + conditionDx,
			H:           outcomeDy,
			Stroke:      fg,
			StrokeWidth: strokeWidth,
			Fill:        fg,
		}
		gateModes       = []string{"RISE", "FALL", "EDGE", "HIGH", "LOW"}
		gateModeStepper = control.Stepper("gate-mode", fg, bg, svg.LargeFont, 9, 4, gateModes)
	)
	p.Engrave(conditionBoxLeft, conditionBoxTop-outcomeDy, headerBox)
	for i := 0; i < nInputs-1; i++ {
		x := conditionX + float64(i)*conditionDx
		y := t.conditionY
		label := svg.TextCentered(t.inputNames[i], svg.LargeFont, bg)
		p.Engrave(x, y, label)
	}
	p.Engrave(t.outcomeX, t.conditionY, svg.TextCentered("Q", svg.LargeFont, bg))
	p.Install(t.outcomeX-conditionDx, t.conditionY, gateModeStepper)

	var (
		outcomeBox = svg.Rect{
			W:           conditionDx,
			H:           conditionBoxHeight,
			Stroke:      fg,
			StrokeWidth: strokeWidth,
			Fill:        fg,
		}
		outcomes       = []string{"T", "F", "Q", "¬Q"}
		outcomeStepper = control.Stepper("outcome", fg, bg, svg.LargeFont, 9, 2, outcomes)
		trueLabel      = svg.TextCentered("T", svg.LargeFont, fg)
		falseLabel     = svg.TextCentered("F", svg.LargeFont, fg)
	)
	p.Engrave(conditionBoxRight, conditionBoxTop, outcomeBox)
	for i := 0; i < nConditions; i++ {
		p.Install(t.outcomeX, t.conditionY+float64(i+1)*outcomeDy, outcomeStepper)
	}

	for condition := 0; condition < nConditions; condition++ {
		y := t.conditionY + float64(condition+1)*outcomeDy
		inputBits := condition
		for input := 1; input <= nInputs; input++ {
			x := t.outcomeX - float64(input)*conditionDx
			if inputBits&1 == 1 {
				p.Engrave(x, y, trueLabel)
			} else {
				p.Engrave(x, y, falseLabel)
			}
			inputBits = inputBits >> 1
		}
	}
	truthOutButtonPort(p, t.outputX, t.outputTop, "Q")
	truthOutButtonPort(p, t.outputX, t.outputTop+t.portDy, "¬ Q")
	return p
}

func truthInButtonPort(p *Panel, x, y float64, name string) {
	port := control.Port(p.Fg, p.Bg)
	button := control.Button(p.Bg, p.Fg)
	p.Install(x, y, port)
	p.Install(x+buttonPortDistance, y, button)

	label := labelAbove(name, port, svg.LargeFont, p.Fg)
	box := boxAround(p.Fg, p.Bg, port, button.DefaultFrame.Translate(buttonPortDistance, 0), label)
	p.Engrave(x, y, box)
	p.Engrave(x, y, label)
}
func truthOutButtonPort(p *Panel, x, y float64, name string) {
	port := control.Port(p.Fg, p.Bg)
	button := control.OutputButton(p.Fg, p.Bg)
	p.Install(x, y, port)
	p.Install(x-buttonPortDistance, y, button)

	label := labelAbove(name, port, svg.LargeFont, p.Bg)
	p.Engrave(x, y, boxAround(p.Fg, p.Fg, port, button.DefaultFrame.Translate(-buttonPortDistance, 0), label))
	p.Engrave(x, y, label)
}
