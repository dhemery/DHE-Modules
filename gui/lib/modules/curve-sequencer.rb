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

hp width_hp

hue = 30
foreground [hue, 100, 10]
background [hue, 10, 93]

left = hp2mm(2)
right = hp2mm(width_hp - 2)
top = hp2mm(3.5)
bottom = hp2mm(23)


###############################################################################
#
# Sequence Controls (Left)
#
###############################################################################

sequence_controls_top = top + hp2mm(2.78)
sequence_controls_bottom = bottom - Port::DIAMETER / 2.0 - 1.0
sequence_controls_dy = (sequence_controls_bottom - sequence_controls_top) / 4
run_y = sequence_controls_top + 0 * sequence_controls_dy
loop_y = sequence_controls_top + 1 * sequence_controls_dy
selection_y = sequence_controls_top + 2 * sequence_controls_dy
gate_y = sequence_controls_top + 3 * sequence_controls_dy
reset_y = sequence_controls_top + 4 * sequence_controls_dy

input_button_port x: left, y: run_y, label: 'RUN'

input_button_port x: left, y: gate_y, label: 'GATE'

selection_length_x = left + hp2mm(1.63)
connector left: left, right: selection_length_x, y: selection_y
small_knob x: left - hp2mm(0.2), y: selection_y, label: 'START'
small_knob x: selection_length_x, y: selection_y, label: 'LENGTH'

input_button_port x: left, y: loop_y, label: 'LOOP'

input_button_port x: left, y: reset_y, label: 'RESET'


###############################################################################
#
# Step Controls
#
###############################################################################

step_x = hp2mm(9.5)
step_dx = hp2mm(2.25)

active_y = top + Light::RADIUS * 1.5
advance_y = top + hp2mm(1.61)
interrupt_y = advance_y + hp2mm(0.8)
hold_y = interrupt_y + hp2mm(0.8)
start_y = top + hp2mm(5.75)
end_y = top + hp2mm(9.5)
duration_y = top + hp2mm(12.35)
curve_y = top + hp2mm(15.25)
enabled_port_y = bottom - Port::DIAMETER / 2.0
enabled_button_y = enabled_port_y - Port::DIAMETER / 2.0 - Button::DIAMETER / 2.0 - 0.5

knob_pick_list_distance = hp2mm(1.15)

label_x = step_x - 0.6 * step_dx
label x: label_x, y: advance_y, text: 'TRIG', alignment: :left_of, size: :large
label x: label_x, y: interrupt_y, text: 'ON INT', alignment: :left_of, size: :large
label x: label_x, y: hold_y, text: 'ON EOS', alignment: :left_of, size: :large
label x: label_x, y: start_y, text: 'START', alignment: :left_of, size: :large
label x: label_x, y: end_y, text: 'END', alignment: :left_of, size: :large
label x: label_x, y: curve_y-hp2mm(0.25), text: 'SHAPE', alignment: :left_of, size: :large
label x: label_x, y: duration_y, text: 'DUR', alignment: :left_of, size: :large
label x: label_x, y: (enabled_button_y + enabled_port_y) / 2, text: 'ON', alignment: :left_of, size: :large

channel_separator_top = top + hp2mm(1.25)

line_x = step_x - step_dx / 2.0
line x1: line_x, x2: line_x, y1: channel_separator_top, y2: bottom

step_label_y = top - hp2mm(0.25)
(0...steps).each do |step|
  x = step_x + step * step_dx
  light x: x, y: active_y
  label x: x, y: step_label_y, text: (step + 1).to_s, alignment: :above, size: :large

  stepper x: x, y: advance_y, name: 'trigger-mode', options: %w[RISE FALL EDGE HIGH LOW], selection: 1, width: 9
  stepper x: x, y: interrupt_y, name: 'on-interrupt', options: %w[NOINT NEXT], selection: 1, width: 9
  stepper x: x, y: hold_y, name: 'on-eos', options: %w[SUST NEXT], selection: 2, width: 9

  stepper x: x, y: start_y - knob_pick_list_distance, name: 'source', options: %w[LEVEL IN OUT], selection: 3, width: 9
  tiny_knob x: x, y: start_y, label: ''
  stepper x: x, y: start_y + knob_pick_list_distance, name: 'track', options: %w[SNAP TRACK], selection: 1, width: 9

  stepper x: x, y: end_y - knob_pick_list_distance, name: 'source', options: %w[LEVEL IN OUT], selection: 1, width: 9
  tiny_knob x: x, y: end_y, label: ''
  stepper x: x, y: end_y + knob_pick_list_distance, name: 'track', options: %w[SNAP TRACK], selection: 2, width: 9

  stepper x: x, y: curve_y - knob_pick_list_distance, name: 'shape', options: %w[S J], selection: 1, width: 9
  tiny_knob y: curve_y, x: x, label: ''

  tiny_knob y: duration_y, x: x, label: ''

  button y: enabled_button_y, x: x, label: ''
  port y: enabled_port_y, x: x, label: ''
  line x1: x, x2: x, y1: enabled_button_y, y2: enabled_port_y

  line_x = x + step_dx / 2.0
  line x1: line_x, x2: line_x, y1: channel_separator_top, y2: bottom
end

position_marker(x: step_x - Light::DIAMETER, y: active_y, type: :start)
position_marker(x: step_x + step_dx * (steps - 1) + Light::DIAMETER, y: active_y, type: :end)


###############################################################################
#
# Sequence Controls (Right)
#
###############################################################################

out_y = bottom - Port::DIAMETER / 2.0 - 1.0
in_y = sequence_controls_top

input_port x: right, y: in_y, label: 'IN'

polarity_y = (start_y + end_y) / 2.0
line x1: line_x, x2: right, y1: start_y, y2: polarity_y
line x1: line_x, x2: right, y1: end_y, y2: polarity_y
polarity_toggle x: right, y: polarity_y, selection: 2

connector left: line_x, right: right, y: duration_y
duration_toggle x: right, y: duration_y

output_port x: right, y: out_y, label: 'OUT'
