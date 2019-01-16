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
  MM_PER_HP = 5.08

  attr_reader :width

  def initialize
    @controls = []
    @faceplate = []
  end

  def build
    DheModule.new(name: @name, controls: @controls, faceplate: @faceplate,
                  width: @width, foreground: @foreground, background: @background)
  end

  def name(name)
    @name = name
  end

  def hp(hp)
    @width = hp * MM_PER_HP
  end

  def foreground(color)
    @foreground = "##{Color::HSL.new(*color).to_rgb.hex}"
  end

  def background(color)
    @background = "##{Color::HSL.new(*color).to_rgb.hex}"
  end

  def separator(y:)
    @faceplate << make_line(left: 0, right: @width, y: y)
  end

  def connector(left:, right:, y:)
    @faceplate << make_line(left: left, right: right, y: y,)
  end

  def button(x:, y:, label: '', style: :normal)
    button = make_button(x: x, y: y, style: style)
    label = make_label(control: button, text: label, size: :small, style: style)

    @faceplate << label
    @controls << button
  end

  def input_port(x:, y:, label: 'IN')
    port = make_port(x: x, y: y)
    label = make_label(control: port, text: label, size: :small)
    @faceplate << make_box(around: [label, port])
    @faceplate << label
    @controls << port
  end

  def output_port(x:, y:, label: 'OUT')
    port = make_port(x: x, y: y)
    label = make_label(control: port, text: label, size: :small, style: :reversed)
    @faceplate << make_box(around: [label, port], style: :solid)
    @faceplate << label
    @controls << port
  end

  def cv_port(x:, y:)
    port = make_port(x: x, y: y)
    label = make_label(control: port, text: 'CV', size: :small)
    @faceplate << label
    @controls << port
  end

  def knob(x:, y:, size:, label:, label_size:)
    knob = make_knob(x: x, y: y, size: size)
    @controls << knob
    @faceplate << make_label(control: knob, text: label, size: label_size)
  end

  def small_knob(x:, y:, label:)
    knob(x: x, y: y, size: :small, label: label, label_size: :small)
  end

  def medium_knob(x:, y:, label:)
    knob(x: x, y: y, size: :medium, label: label, label_size: :small)
  end

  def large_knob(x:, y:, label:)
    knob(x: x, y: y, size: :large, label: label, label_size: :large)
  end

  def attenuverter(x:, y:)
    knob(x: x, y: y, size: :tiny, label: '<tspan font-size="larger">-&#160;&#160;+</tspan>', label_size: :large)
  end

  def toggle(x:, y:, labels:, selection:)
    toggle = make_toggle(x: x, y: y,
                         size: labels.size, selection: selection)
    @controls << toggle
    @faceplate << make_label(control: toggle, alignment: :below,
                             text: labels.first, size: :small)
    @faceplate << make_label(control: toggle, alignment: :right_of,
                             text: labels[1], size: :small) if labels.size == 3
    @faceplate << make_label(control: toggle, alignment: :above,
                             text: labels.last, size: :small)
  end

  def polarity_toggle(x:, y:, selection: 1)
    toggle(x: x, y: y, labels: %w(BI UNI), selection: selection)
  end

  def duration_toggle(x:, y:)
    toggle(x: x, y: y, labels: %w(1 10 100), selection: 2)
  end

  def shape_toggle(x:, y:)
    toggle(x: x, y: y, labels: %w(J S), selection: 1)
  end

  def counter(x:, y:, name:, labels:, selection: 1, enabled: true)
    @controls << make_counter(x: x, y: y, name: name, labels: labels, selection: selection, enabled: enabled)
  end

  def input_button_port(x:, y:, label:)
    port = make_port(x: x, y: y)
    label = make_label(control: port, text: label, size: :small)
    button_x = port.right + PADDING + Button::DIAMETER / 2.0
    button = make_button(x: button_x, y: y)
    @faceplate << make_box(around: [button, label, port])
    @faceplate << label
    @controls << port
    @controls << button
  end

  def output_button_port(x:, y:, label:)
    port = make_port(x: x, y: y)
    label = make_label(control: port, text: label, size: :small, style: :reversed)
    button_x = port.left - PADDING - Button::DIAMETER / 2.0
    button = make_button(x: button_x, y: y, style: :reversed)
    @faceplate << make_box(around: [button, label, port], style: :solid)
    @faceplate << label
    @controls << port
    @controls << button
  end

  def make_box(around:, style: :outline)
    stroke_inset = STROKE_WIDTH / 2.0
    box_buffer = PADDING + stroke_inset
    top = around.map(&:top).min - box_buffer
    right = around.map(&:right).max + box_buffer
    bottom = around.map(&:bottom).max + box_buffer
    left = around.map(&:left).min - box_buffer
    stroke = @foreground
    fill = style == :solid ? @foreground : @background
    Box.new(top: top, right: right, bottom: bottom, left: left,
            stroke: stroke, fill: fill)
  end

  def make_button(x:, y:, style: :normal)
    ring_color = style == :normal ? @foreground : @background
    pressed_color = style == :normal ? @background : @foreground
    Button.new(x: x, y: y, ring_color: ring_color, pressed_color: pressed_color, style: style)
  end

  def make_counter(x:, y:, name:, labels:, selection:, enabled:)
    Counter.new(x: x, y: y, foreground: @foreground, background: @background,
                name: name, labels: labels, selection: selection, enabled: enabled)
  end

  def make_knob(x:, y:, size:)
    Knob.new(x: x, y: y, knob_color: @foreground, pointer_color: @background, size: size,)
  end

  def make_label(control:, text:, size:, alignment: :above, style: :normal)
    color = style == :normal ? @foreground : @background
    case alignment
      when :above
        x = control.x
        y = control.top - PADDING
      when :right_of
        x = control.right + PADDING / 2
        y = control.y
      when :below
        x = control.x
        y = control.bottom + PADDING
      else
        raise "unknown alignment #{alignment} for label #{text}"
    end
    Label.new(x: x, y: y, text: text, color: color, size: size, alignment: alignment)
  end

  def make_line(left:, right:, y:)
    Line.new(x1: left, x2: right, y1: y, y2: y, color: @foreground)
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