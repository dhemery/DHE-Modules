require 'color'

require_relative 'dhe_module'
require_relative 'dimensions'
require_relative 'controls/box'
require_relative 'controls/button'
require_relative 'controls/counter'
require_relative 'controls/knob'
require_relative 'controls/label'
require_relative 'controls/line'
require_relative 'controls/port'
require_relative 'controls/toggle'

class ModuleFactory
  BOX_BUFFER = PADDING + STROKE_INSET

  def initialize
    @controls = []
    @faceplate = []
  end

  def build
    DheModule.new(name: @name, hp: @hp, foreground: @foreground, background: @background, controls: @controls,
                  faceplate: @faceplate)
  end

  def name(name)
    @name = name
  end

  def hp(hp)
    @hp = hp
    @width = hp * MM_PER_HP
    @width_px = @width * PX_PER_MM
  end

  def foreground(color)
    @foreground = "##{Color::HSL.new(*color).to_rgb.hex}"
  end

  def background(color)
    @background = "##{Color::HSL.new(*color).to_rgb.hex}"
  end

  def attenuverter(x:, y:)
    knob(x: x, y: y, size: :tiny, label: '<tspan font-size="larger">-&#160;&#160;+</tspan>', label_size: :large)
  end

  def button(x:, y:, label: '', style: :foreground)
    button = make_button(x: x, y: y, style: style)
    label = make_label(text: label, size: :small, x: x, y: button.top - PADDING, style: style)
    @faceplate << label
    @controls << button
  end

  def connector(left:, right:, y:)
    @faceplate << make_line(x1: left, y1: y, x2: right)
  end

  def counter(x:, y:, name:, labels:, selection: 1, enabled: true)
    @controls << make_counter(x: x, y: y, name: name, labels: labels, selection: selection, enabled: enabled)
  end

  def cv_port(x:, y:)
    port = make_port(x: x, y: y)
    label = make_label(text: 'CV', size: :small, x: x, y: port.top - PADDING)
    @faceplate << label
    @controls << port
  end

  def duration_toggle(x:, y:)
    toggle(x: x, y: y, labels: %w(1 10 100), selection: 2)
  end

  def input_port(x:, y:, label: 'IN')
    port = make_port(x: x, y: y)
    label = make_label(text: label, size: :small, x: x, y: port.top - PADDING)
    @faceplate << make_box(top: label.top, right: port.right, bottom: port.bottom, left: port.left)
    @faceplate << label
    @controls << port
  end

  def knob(x:, y:, label:, size:, label_size:)
    knob = make_knob(x: x, y: y, size: size)
    @controls << knob
    @faceplate << make_label(text: label, size: label_size, x: x, y: knob.top - PADDING)
  end

  def large_knob(x:, y:, label:)
    knob(x: x, y: y, size: :large, label: label, label_size: :large)
  end

  def medium_knob(x:, y:, label:)
    knob(x: x, y: y, size: :medium, label: label, label_size: :small)
  end

  def output_port(x:, y:, label: 'OUT')
    port = make_port(x: x, y: y)
    label = make_label(text: label, size: :small, style: :background, x: x, y: port.top - PADDING)
    @faceplate << make_box(top: label.top, right: port.right, bottom: port.bottom, left: port.left, style: :solid)
    @faceplate << label
    @controls << port
  end

  def polarity_toggle(x:, y:, selection: 1)
    toggle(x: x, y: y, labels: %w(BI UNI), selection: selection)
  end

  def separator(y:)
    @faceplate << make_line(x1: 0, y1: y, x2: @width)
  end

  def shape_toggle(x:, y:)
    toggle(x: x, y: y, labels: %w(J S), selection: 1)
  end

  def small_knob(x:, y:, label:)
    knob(x: x, y: y, size: :small, label: label, label_size: :small)
  end

  def toggle(x:, y:, labels:, selection:)
    toggle = make_toggle(x: x, y: y,
                         size: labels.size, selection: selection)
    @controls << toggle
    @faceplate << make_label(text: labels.first, size: :small,
                             x: x, y: toggle.bottom + PADDING,
                             alignment: :below)
    @faceplate << make_label(text: labels[1], size: :small,
                             x: toggle.right + PADDING / 2.0, y: y,
                             alignment: :right_of) if labels.size == 3
    @faceplate << make_label(text: labels.last, size: :small,
                             x: x, y: toggle.top - PADDING,
                             alignment: :above)
  end

  def input_button_port(x:, y:, label:)
    port = make_port(x: x, y: y)
    label = make_label(text: label, size: :small, x: x, y: port.top - PADDING)
    button_x = port.right + PADDING + Button::DIAMETER / 2.0
    button = make_button(x: button_x, y: y)
    @faceplate << make_box(top: label.top, right: button.right, bottom: port.bottom, left: port.left)
    @faceplate << label
    @controls << port
    @controls << button
  end

  def output_button_port(x:, y:, label:)
    port = make_port(x: x, y: y)
    label = make_label(text: label, size: :small, x: x, y: port.top - PADDING, style: :background)
    button_x = port.left - PADDING - Button::DIAMETER / 2.0
    button = make_button(x: button_x, y: y, style: :background)
    @faceplate << make_box(top: label.top, right: port.right, bottom: port.bottom, left: button.left, style: :solid)
    @faceplate << label
    @controls << port
    @controls << button
  end

  def make_box(top:, right:, bottom:, left:, style: :outline)
    stroke = @foreground
    fill = style == :solid ? @foreground : @background
    Box.new(top: top - BOX_BUFFER, right: right + PADDING, bottom: bottom + PADDING, left: left - PADDING,
            stroke: stroke, fill: fill)
  end

  def make_button(x:, y:, style: :foreground)
    ring_color = @foreground
    pressed_color = @background
    ring_color, pressed_color = pressed_color, ring_color if style == :background
    Button.new(x: x, y: y, ring_color: ring_color, pressed_color: pressed_color)
  end

  def make_counter(x:, y:, name:, labels:, selection:, enabled:)
    Counter.new(x: x, y: y, foreground: @foreground, background: @background,
                name: name, labels: labels, selection: selection, enabled: enabled)
  end

  def make_knob(x:, y:, size:)
    Knob.new(x: x, y: y, knob_color: @foreground, pointer_color: @background, size: size,)
  end

  def make_label(x:, y:, text:, size:, alignment: :above, style: :foreground)
    color = style == :background ? @background : @foreground
    Label.new(x: x, y: y, text: text, color: color, size: size, alignment: alignment)
  end

  def make_line(x1:, x2: x1, y1:, y2: y1)
    Line.new(x1: x1, x2: x2, y1: y1, y2: y2, color: @foreground)
  end

  def make_port(x:, y:)
    Port.new(x: x, y: y, foreground: @foreground, background: @background)
  end

  def make_toggle(x:, y:, size:, selection:)
    Toggle.new(x: x, y: y,
               foreground: @foreground, background: @background,
               size: size, selection: selection)
  end
end