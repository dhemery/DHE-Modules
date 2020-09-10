STEPS = 4
STEP_BLOCK_WIDTH = 9.0
STEP_DX = STEP_BLOCK_WIDTH / STEPS

class PositionMarker < Shape
  attr_reader :slug
  TOP = -Light::DIAMETER
  BOTTOM = Light::DIAMETER
  RIGHT = Light::DIAMETER
  LEFT = -Light::DIAMETER
  NIB = Light::DIAMETER
  THICKNESS = STROKE_WIDTH

  def initialize(type:, color:)
    @color = color
    @slug = Pathname("marker-#{type}")
    @outer = type == :start ? LEFT : RIGHT
    @inner = type == :start ? LEFT + NIB : RIGHT - NIB
    super(top: TOP, right: RIGHT, bottom: BOTTOM, left: LEFT)
  end

  def draw(canvas)
    canvas.line(x1: @outer, y1: TOP, x2: @outer, y2: BOTTOM, 'stroke-width' => THICKNESS, 'stroke-linecap' => "square", stroke: @color)
    canvas.line(x1: @outer, y1: TOP, x2: @inner, y2: TOP, 'stroke-width' => THICKNESS, 'stroke-linecap' => "square", stroke: @color)
    canvas.line(x1: @outer, y1: BOTTOM, x2: @inner, y2: BOTTOM, 'stroke-width' => THICKNESS, 'stroke-linecap' => "square", stroke: @color)
  end

  def frames
    [self]
  end
end

def position_marker(x:, y:, type:)
  marker = PositionMarker.new(type: type, color: @foreground)
  @controls << marker
  @image_shapes << marker.translated(x, y)
end

def active_y(top)
  top + Light::RADIUS * 1.5
end

def trigger_y(top)
  trigger_y = top + hp2mm(1.61)
end

def interrupt_y(top)
  trigger_y(top) + hp2mm(0.8)
end

def completion_y(top)
  interrupt_y(top) + hp2mm(0.8)
end

def start_anchor_y(top)
  top + hp2mm(5.75)
end

def end_anchor_y(top)
  top + hp2mm(9.5)
end

def duration_y(top)
  top + hp2mm(12.35)
end

def shape_y(top)
  top + hp2mm(15.25)
end

def enabled_button_y(bottom)
  bottom - Port::DIAMETER / 2.0
end

def step_block(top:, left:, bottom:, foreground:, background:)
  step_x = left

  knob_pick_list_distance = hp2mm(1.15)

  channel_separator_top = top + hp2mm(1.25)

  line_x = step_x - STEP_DX / 2.0
  line x1: line_x, x2: line_x, y1: channel_separator_top, y2: bottom

  stepper_width = 9
  anchor_sources = %w[LEVEL IN OUT AUX]
  anchor_modes = %w[SAMPL TRACK]
  step_label_y = top - hp2mm(0.25)
  (0...STEPS).each do |step|
    x = step_x + step * STEP_DX
    light x: x, y: active_y(top)
    label x: x, y: step_label_y, text: (step + 1).to_s, alignment: :above, size: :large

    stepper x: x, y: trigger_y(top), name: 'trigger-mode', options: %w[RISE FALL EDGE HIGH LOW], selection: 1, width: stepper_width
    stepper x: x, y: interrupt_y(top), name: 'interrupt-mode', options: %w[IGNOR NEXT], selection: 1, width: stepper_width
    stepper x: x, y: completion_y(top), name: 'completion-mode', options: %w[SUST NEXT], selection: 2, width: stepper_width

    stepper x: x, y: start_anchor_y(top) - knob_pick_list_distance, name: 'anchor-mode', options: anchor_modes, selection: 1, width: stepper_width
    tiny_knob x: x, y: start_anchor_y(top), label: ''
    stepper x: x, y: start_anchor_y(top) + knob_pick_list_distance, name: 'anchor-source', options: anchor_sources, selection: 3, width: stepper_width

    stepper x: x, y: end_anchor_y(top) - knob_pick_list_distance, name: 'anchor-mode', options: anchor_modes, selection: 2, width: stepper_width
    tiny_knob x: x, y: end_anchor_y(top), label: ''
    stepper x: x, y: end_anchor_y(top) + knob_pick_list_distance, name: 'anchor-source', options: anchor_sources, selection: 1, width: stepper_width

    stepper x: x, y: shape_y(top) - knob_pick_list_distance, name: 'shape', options: %w[J S], selection: 1, width: stepper_width
    tiny_knob y: shape_y(top), x: x, label: ''

    tiny_knob y: duration_y(top), x: x, label: ''

    button y: enabled_button_y(bottom), x: x, label: ''

    line_x = x + STEP_DX / 2.0
    line x1: line_x, x2: line_x, y1: channel_separator_top, y2: bottom
  end

  position_marker(x: step_x - Light::DIAMETER, y: active_y(top), type: :start)
  position_marker(x: step_x + STEP_DX * (STEPS - 1) + Light::DIAMETER, y: active_y(top), type: :end)
end
