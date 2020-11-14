require 'color'
require 'pathname'

require_relative 'dimensions'
require_relative 'shapes/box'
require_relative 'shapes/label'
require_relative 'shapes/line'
require_relative 'shapes/light'

require_relative 'controls/button'
require_relative 'controls/knob'
require_relative 'controls/pick_list'
require_relative 'controls/port'
require_relative 'controls/stepper'
require_relative 'controls/toggle'

class ModuleFactory
  MODULE_LABEL_INSET = 9.0

  attr_reader :source_file, :width, :slug, :faceplate_shape, :overlay_shape, :controls

  def initialize(source_file)
    @source_file = Pathname(source_file)
    @slug = Pathname(source_file.to_s.pathmap('%n'))
    @faceplate_shapes = []
    @overlay_shapes = []
    @controls = []
  end

  def build
    instance_eval(@source_file.read, @source_file.to_s)
    faceplate_stroke_width = 0.5
    faceplate_inset = faceplate_stroke_width / 2.0
    faceplate_background = Box.new(top: faceplate_inset, left: faceplate_inset,
                                   right: @width - faceplate_inset, bottom: MODULE_HEIGHT - faceplate_inset,
                                   fill: @background, stroke: @foreground,
                                   corner_radius: 0.0, stroke_width: faceplate_stroke_width)
    module_label = Label.new(text: @name, size: :title, color: @foreground)
                        .translated(width / 2, MODULE_LABEL_INSET)
    author_label = Label.new(text: 'DHE', size: :title, color: @foreground, alignment: :below)
                        .translated(width / 2, MODULE_HEIGHT - MODULE_LABEL_INSET)
    @faceplate_shapes.prepend(faceplate_background, module_label, author_label)
    @faceplate_shape = CompositeShape.new(@faceplate_shapes)
    @overlay_shape = CompositeShape.new(@overlay_shapes)
    self
  end

  def name(name)
    @name = name
  end

  def hp(hp)
    @width = hp2mm(hp)
  end

  def foreground(color)
    @foreground = "##{Color::HSL.new(*color).to_rgb.hex}"
  end

  def background(color)
    @background = "##{Color::HSL.new(*color).to_rgb.hex}"
  end

  def line(x1:, y1:, x2:, y2:)
    @faceplate_shapes << Line.new(x1: x1, x2: x2, y1: y1, y2: y2, stroke: @foreground, width: STROKE_WIDTH)
  end

  def separator(y:)
    line(x1: 0, y1: y, x2: @width, y2: y)
  end

  def connector(left:, right:, y:)
    line(x1: left, x2: right, y1: y, y2: y)
  end

  def port(x:, y:, label: '')
    port = Port.new(metal_color: @background, shadow_color: @foreground)
    @controls << port

    port_for_overlay = port.translated(x, y)
                           .padded(all: PADDING)
    @overlay_shapes << port_for_overlay

    faceplate_label = Label.new(text: label, color: @foreground, size: :small)
                           .translated(port_for_overlay.x, port_for_overlay.top)
    @faceplate_shapes << faceplate_label
  end

  def cv_port(x:, y:)
    port(x: x, y: y, label: 'CV')
  end

  def box(top:, right:, bottom:, left:, fill: @background, stroke: @foreground,
          stroke_width: STROKE_WIDTH, corner_radius: Box::CORNER_RADIUS)
    @faceplate_shapes << Box.new(top: top, right: right, bottom: bottom, left: left,
                                 stroke: stroke, fill: fill,
                                 stroke_width: stroke_width, corner_radius: corner_radius)
  end

  def input_port(x:, y:, label: 'IN')
    port = Port.new(metal_color: @background, shadow_color: @foreground)
    @controls << port

    port_for_overlay = port.translated(x, y)
                           .padded(all: PADDING)
    @overlay_shapes << port_for_overlay

    faceplate_label = Label.new(text: label, color: @foreground, size: :small)
                           .translated(port_for_overlay.x, port_for_overlay.top)
                           .padded(top: PADDING, right: 0.0, bottom: 0.0)

    faceplate_box = Box.around(shapes: [faceplate_label, port_for_overlay],
                               stroke: @foreground, stroke_width: STROKE_WIDTH, fill: @background)
    @faceplate_shapes << faceplate_box
    @faceplate_shapes << faceplate_label
  end

  def output_port(x:, y:, label: 'OUT')
    port = Port.new(metal_color: @background, shadow_color: @foreground)
    @controls << port

    port_for_overlay = port.translated(x, y)
                           .padded(all: PADDING)
    @overlay_shapes << port_for_overlay

    faceplate_label = Label.new(text: label, color: @background, size: :small)
                           .translated(port_for_overlay.x, port_for_overlay.top)
                           .padded(top: PADDING, right: 0.0, bottom: 0.0)

    faceplate_box = Box.around(shapes: [faceplate_label, port_for_overlay],
                               stroke: @foreground, stroke_width: STROKE_WIDTH, fill: @foreground)

    @faceplate_shapes << faceplate_box
    @faceplate_shapes << faceplate_label
  end

  def knob(x:, y:, size:, label: nil, label_size:)
    knob = Knob.new(knob_color: @foreground, pointer_color: @background, size: size)
    @controls << knob

    knob_for_overlay = knob.translated(x, y)
                           .padded(top: PADDING, right: 0.0, bottom: 0.0)
    @overlay_shapes << knob_for_overlay

    return if label.nil?

    faceplate_label = Label.new(text: label, color: @foreground, size: label_size)
                           .translated(knob_for_overlay.x, knob_for_overlay.top)
    @faceplate_shapes << faceplate_label
  end

  def tiny_knob(x:, y:, label: nil)
    knob(x: x, y: y, size: :tiny, label: label, label_size: :small)
  end

  def small_knob(x:, y:, label: nil)
    knob(x: x, y: y, size: :small, label: label, label_size: :small)
  end

  def medium_knob(x:, y:, label: nil)
    knob(x: x, y: y, size: :medium, label: label, label_size: :small)
  end

  def large_knob(x:, y:, label: nil)
    knob(x: x, y: y, size: :large, label: label, label_size: :large)
  end

  def attenuverter(x:, y:)
    knob(x: x, y: y, size: :tiny, label: '- +', label_size: :large)
  end

  def toggle(x:, y:, labels:, selection:)
    toggle = Toggle::new(foreground: @foreground, background: @background, size: labels.size)
    @controls << toggle

    switch_for_overlay = toggle.states[selection - 1]
                               .translated(x, y)
                               .padded(all: PADDING)
    @overlay_shapes << switch_for_overlay

    @faceplate_shapes << Label.new(text: labels.first, color: @foreground, size: :small, alignment: :below)
                              .translated(switch_for_overlay.x, switch_for_overlay.bottom)

    @faceplate_shapes << Label.new(text: labels.last, color: @foreground, size: :small, alignment: :above)
                              .translated(switch_for_overlay.x, switch_for_overlay.top)

    if labels.size == 3
      @faceplate_shapes << Label.new(text: labels[1], color: @foreground, size: :small, alignment: :right_of)
                                .translated(switch_for_overlay.right, switch_for_overlay.y)
    end
  end

  def polarity_toggle(x:, y:, selection: 1)
    toggle(x: x, y: y, labels: %w[BI UNI], selection: selection)
  end

  def duration_toggle(x:, y:)
    toggle(x: x, y: y, labels: %w[1 10 100], selection: 2)
  end

  def shape_toggle(x:, y:)
    toggle(x: x, y: y, labels: %w[J S], selection: 1)
  end

  def pick_list(x:, y:, name:, options:, selection: 1, size: :small, width:, hidden: false)
    picklist = PickList.new(name: name, options: options, size: size, text_color: @foreground, fill: @background, width: width)

    @controls << picklist

    option_for_overlay = picklist.options[selection - 1]
                                 .translated(x, y)
                                 .padded(all: PADDING)

    @overlay_shapes << option_for_overlay unless hidden
  end

  def stepper(x:, y:, name:, options:, selection: 0, width:, hidden: false)
    stepper = Stepper.new(name: name, options: options, text_color: @foreground, fill: @background, width: width)

    @controls << stepper

    option_for_overlay = stepper.options[selection - 1]
                                .translated(x, y)
                                .padded(all: PADDING)

    @overlay_shapes << option_for_overlay unless hidden
  end

  def button(x:, y:, label: nil, name: 'button')
    button = Button.new(name: name, pressed_color: @background, released_color: @foreground)
    @controls << button

    button_for_overlay = button.released
                               .translated(x, y)
                               .padded(all: PADDING)
    @overlay_shapes << button_for_overlay

    return if label.nil?

    faceplate_label = Label.new(text: label, color: @foreground, size: :small, alignment: :above)
                           .translated(button_for_overlay.x, button_for_overlay.top)
    @faceplate_shapes << faceplate_label
  end

  def input_button_port(x:, y:, label:)
    port = Port.new(metal_color: @background, shadow_color: @foreground)
    @controls << port

    button = Button.new(pressed_color: @background, released_color: @foreground)
    @controls << button

    port_for_overlay = port.translated(x, y)
                           .padded(all: PADDING)
    @overlay_shapes << port_for_overlay

    button_for_overlay = button.released
                               .translated(port_for_overlay.right + button.released.right, port_for_overlay.y)
                               .padded(all: PADDING, left: 0.0)
    @overlay_shapes << button_for_overlay

    faceplate_label = Label.new(text: label, color: @foreground, size: :small)
                           .translated(port_for_overlay.x, port_for_overlay.top)
                           .padded(top: PADDING)

    faceplate_box = Box.around(shapes: [port_for_overlay, button_for_overlay, faceplate_label],
                               stroke: @foreground, stroke_width: STROKE_WIDTH, fill: @background)
    @faceplate_shapes << faceplate_box
    @faceplate_shapes << faceplate_label
    faceplate_box
  end

  def output_button_port(x:, y:, label:)
    port = Port.new(metal_color: @background, shadow_color: @foreground)
    @controls << port

    button = Button.new(name: 'output-button', pressed_color: @foreground, released_color: @background)
    @controls << button

    port_for_overlay = port.translated(x, y)
                           .padded(all: PADDING)
    @overlay_shapes << port_for_overlay

    button_for_overlay = button.released.translated(port_for_overlay.left + button.released.left, port_for_overlay.y)
                               .padded(all: PADDING, right: 0.0)
    @overlay_shapes << button_for_overlay

    faceplate_label = Label.new(text: label, color: @background, size: :small)
                           .translated(port_for_overlay.x, port_for_overlay.top)
                           .padded(top: PADDING)

    faceplate_box = Box.around(shapes: [port_for_overlay, button_for_overlay, faceplate_label],
                               stroke: @foreground, stroke_width: STROKE_WIDTH, fill: @foreground)
    @faceplate_shapes << faceplate_box
    @faceplate_shapes << faceplate_label
  end

  def label(x:, y:, text:, color: @foreground, size: :small, alignment: :right_of)
    @faceplate_shapes << Label.new(text: text, color: color, size: size, alignment: alignment)
                              .translated(x, y)
  end

  def light(x:, y:, color: @foreground)
    @faceplate_shapes << Light.new(color: color).translated(x, y)
  end
end
