# frozen_string_literal: true

require_relative '../../lib/shapes/position-marker'
require_relative '../../lib/dimensions'

HUE = 30
FOREGROUND_HSL = [HUE, 100, 10]
BACKGROUND_HSL = [HUE, 10, 93]

def make_sequence_foundry(steps)
  name "SEQUENCE FOUNDRY #{steps}"
  content_width_hp = mm2hp(content_width(steps))
  module_width_hp = (content_width_hp + 2.0).round
  hp module_width_hp

  foreground FOREGROUND_HSL
  background BACKGROUND_HSL

  x_offset = hp2mm((module_width_hp - content_width_hp) / 2.0)
  make_sequence_controls(x_offset)
  make_global_step_controls(x_offset)
  make_step_control_labels(x_offset)
  make_step_block(steps, x_offset)
  # horizontal(TOP)
  # horizontal(BOTTOM)
end

TOP = 23
BOTTOM = 117.0
MARGIN = 6.0
STEP_WIDTH = 13.0

SEQUENCE_CONTROLS_LEFT = 0.0
SEQUENCE_CONTROLS_WIDTH = PADDING + Port::DIAMETER + PADDING + Button::DIAMETER + PADDING
SEQUENCE_CONTROLS_RIGHT = SEQUENCE_CONTROLS_LEFT + SEQUENCE_CONTROLS_WIDTH

GLOBAL_STEP_CONTROLS_COLUMN_WIDTH = PADDING + Port::DIAMETER + PADDING
GLOBAL_STEP_CONTROLS_LEFT = SEQUENCE_CONTROLS_RIGHT + MARGIN
GLOBAL_STEP_CONTROLS_WIDTH = GLOBAL_STEP_CONTROLS_COLUMN_WIDTH + PADDING + GLOBAL_STEP_CONTROLS_COLUMN_WIDTH + PADDING + GLOBAL_STEP_CONTROLS_COLUMN_WIDTH
GLOBAL_STEP_CONTROLS_RIGHT = GLOBAL_STEP_CONTROLS_LEFT + GLOBAL_STEP_CONTROLS_WIDTH

LABELS_LEFT = GLOBAL_STEP_CONTROLS_RIGHT + MARGIN
LABELS_WIDTH = 0.67 * STEP_WIDTH
LABELS_RIGHT = LABELS_LEFT + LABELS_WIDTH

STEP_BLOCK_LEFT = LABELS_RIGHT + PADDING

def content_width(steps)
  STEP_BLOCK_LEFT + steps * STEP_WIDTH
end

# The distance from the top of the port box to the center of the port
PORT_BOX_ASCENT = PADDING + Label::SIZES[:small] + Port::RADIUS + 0.22
# The distance from the bottom of the port box to the center of the port
PORT_BOX_DESCENT = PADDING + Port::RADIUS
GLOBAL_CONTROLS_TOP_Y = TOP + PORT_BOX_ASCENT
GLOBAL_CONTROLS_BOTTOM_Y = BOTTOM - PORT_BOX_DESCENT
GLOBAL_CONTROLS_DY = (GLOBAL_CONTROLS_BOTTOM_Y - GLOBAL_CONTROLS_TOP_Y) / 4

def global_controls_y(row)
  GLOBAL_CONTROLS_TOP_Y + row * GLOBAL_CONTROLS_DY
end

def make_sequence_controls(x_offset)
  x = SEQUENCE_CONTROLS_LEFT + Port::RADIUS + PADDING + x_offset

  run_y = global_controls_y(0)
  loop_y = global_controls_y(1)
  selection_y = global_controls_y(2)
  gate_y = global_controls_y(3)
  reset_y = global_controls_y(4)

  selection_length_x_offset = hp2mm(1.63)
  selection_length_x = x + selection_length_x_offset

  input_button_port x: x, y: run_y, label: 'RUN'
  input_button_port x: x, y: gate_y, label: 'GATE'
  connector left: x, right: selection_length_x, y: selection_y
  small_knob x: x - hp2mm(0.2), y: selection_y, label: 'START'
  small_knob x: selection_length_x, y: selection_y, label: 'LENGTH'
  input_button_port x: x, y: loop_y, label: 'LOOP'
  input_button_port x: x, y: reset_y, label: 'RESET'
end

def vertical(x)
  line x1: x, x2: x, y1: TOP, y2: BOTTOM
end

def horizontal(y)
  line x1: 0, x2: width, y1: y, y2: y
end

def make_global_step_controls(x_offset)
  left_x = GLOBAL_STEP_CONTROLS_LEFT + GLOBAL_STEP_CONTROLS_COLUMN_WIDTH / 2.0 + x_offset
  center_x = left_x + GLOBAL_STEP_CONTROLS_COLUMN_WIDTH + PADDING
  right_x = center_x + GLOBAL_STEP_CONTROLS_COLUMN_WIDTH + PADDING

  level_y = global_controls_y(0)
  small_knob x: left_x, y: level_y, label: 'LEVEL'
  port x: center_x, y: level_y
  polarity_toggle x: right_x, y: level_y, selection: 2
  connector left: left_x, right: right_x, y: level_y

  duration_y = global_controls_y(1)
  small_knob x: left_x, y: duration_y, label: 'DUR'
  port x: center_x, y: duration_y
  duration_toggle x: right_x, y: duration_y
  connector left: left_x, right: right_x, y: duration_y

  in_y = global_controls_y(2)
  input_port x: left_x, y: in_y, label: 'IN A'
  input_port x: center_x, y: in_y, label: 'IN B'
  input_port x: right_x, y: in_y, label: 'TRIG'

  trig_y = global_controls_y(3)
  output_port x: left_x, y: trig_y, label: 'STEP'
  output_port x: center_x, y: trig_y, label: 'CURVE'
  output_port x: right_x, y: trig_y, label: 'SUST'

  out_y = global_controls_y(4)
  output_port x: left_x, y: out_y, label: 'STEP Δ'
  output_port x: center_x, y: out_y, label: 'SEQ Δ'
  output_port x: right_x, y: out_y, label: 'OUT'
end

STEPPER_WIDTH = STEP_WIDTH * 0.8
# The distance from the stepper baseline to the top of its box
STEPPER_ASCENT = Label::SIZES[:small] / 2.0 + PADDING - 0.25
STEPPER_HEIGHT = STEPPER_ASCENT * 2.0

INTRA_SECTION_GLUE = 0.5
INTER_SECTION_GLUE = 4.0

PROGRESS_LIGHT_Y = TOP - Light::DIAMETER * 2.0
STEP_NUMBER_LABEL_Y = PROGRESS_LIGHT_Y - Light::DIAMETER * 1.5

TRIGGER_Y = TOP + STEPPER_ASCENT
INTERRUPT_Y = TRIGGER_Y + STEPPER_HEIGHT + INTRA_SECTION_GLUE
SUSTAIN_Y = INTERRUPT_Y + STEPPER_HEIGHT + INTRA_SECTION_GLUE

START_ANCHOR_MODE_Y = SUSTAIN_Y + STEPPER_HEIGHT + INTER_SECTION_GLUE
START_ANCHOR_LEVEL_Y = START_ANCHOR_MODE_Y+ (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE
START_ANCHOR_SOURCE_Y = START_ANCHOR_LEVEL_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE

END_ANCHOR_MODE_Y = START_ANCHOR_SOURCE_Y + STEPPER_HEIGHT + INTER_SECTION_GLUE
END_ANCHOR_LEVEL_Y = END_ANCHOR_MODE_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE
END_ANCHOR_SOURCE_Y = END_ANCHOR_LEVEL_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE

DURATION_Y = END_ANCHOR_SOURCE_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTER_SECTION_GLUE

SHAPE_Y = DURATION_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTER_SECTION_GLUE
CURVATURE_Y = SHAPE_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE

ENABLED_Y = BOTTOM - Button::DIAMETER / 2.0

START_ANCHOR_LABEL_Y = (START_ANCHOR_SOURCE_Y + START_ANCHOR_MODE_Y) / 2.0
END_ANCHOR_LABEL_Y = (END_ANCHOR_SOURCE_Y + END_ANCHOR_MODE_Y) / 2.0
SHAPE_LABEL_Y = (SHAPE_Y + CURVATURE_Y) / 2.0

def make_step_control_labels(x_offset)
  label_x = LABELS_RIGHT + x_offset
  label x: label_x, y: TRIGGER_Y, text: 'TRIG', alignment: :left_of, size: :large
  label x: label_x, y: INTERRUPT_Y, text: 'INT', alignment: :left_of, size: :large
  label x: label_x, y: SUSTAIN_Y, text: 'SUST', alignment: :left_of, size: :large
  label x: label_x, y: START_ANCHOR_LABEL_Y, text: 'START', alignment: :left_of, size: :large
  label x: label_x, y: END_ANCHOR_LABEL_Y, text: 'END', alignment: :left_of, size: :large
  label x: label_x, y: DURATION_Y, text: 'DUR', alignment: :left_of, size: :large
  label x: label_x, y: SHAPE_LABEL_Y - hp2mm(0.25), text: 'SHAPE', alignment: :left_of, size: :large
  label x: label_x, y: ENABLED_Y, text: 'ON', alignment: :left_of, size: :large
end

ANCHOR_SOURCES = ["LEVEL", "IN A", "IN B", "OUT"]
ANCHOR_MODES = %w[SAMPLE TRACK]
START_ANCHOR_MODE_SELECTION = 1
START_ANCHOR_SOURCE_SELECTION = 4
END_ANCHOR_MODE_SELECTION = 2
END_ANCHOR_SOURCE_SELECTION = 1
COMPLETION_MODES = %w[NO YES]
COMPLETION_MODE_SELECTION = 1
INTERRUPT_MODES = %w[IGNORE ADVANCE]
INTERRUPT_MODE_SELECTION = 1
SHAPE_OPTIONS = %w[J S]
SHAPE_SELECTION = 1
TRIGGER_MODES = %w[RISE FALL EDGE HIGH LOW]
TRIGGER_MODE_SELECTION = 1

def make_step_block(steps, x_offset)
  step_block_x = STEP_BLOCK_LEFT + x_offset

  line x1: step_block_x, x2: step_block_x, y1: TOP, y2: BOTTOM

  (0...steps).each do |step|
    step_left = step_block_x + step * STEP_WIDTH
    step_x = step_left + STEP_WIDTH / 2.0
    step_right = step_left + STEP_WIDTH
    line x1: step_right, x2: step_right, y1: TOP, y2: BOTTOM

    light x: step_x, y: PROGRESS_LIGHT_Y
    label x: step_x, y: STEP_NUMBER_LABEL_Y, text: (step + 1).to_s, alignment: :above, size: :large

    stepper x: step_x, y: TRIGGER_Y, name: 'trigger-mode', options: TRIGGER_MODES, selection: TRIGGER_MODE_SELECTION, width: STEPPER_WIDTH
    stepper x: step_x, y: INTERRUPT_Y, name: 'interrupt-mode', options: INTERRUPT_MODES, selection: INTERRUPT_MODE_SELECTION, width: STEPPER_WIDTH
    stepper x: step_x, y: SUSTAIN_Y, name: 'completion-mode', options: COMPLETION_MODES, selection: COMPLETION_MODE_SELECTION, width: STEPPER_WIDTH

    stepper x: step_x, y: START_ANCHOR_MODE_Y, name: 'anchor-mode', options: ANCHOR_MODES, selection: START_ANCHOR_MODE_SELECTION, width: STEPPER_WIDTH
    small_knob x: step_x, y: START_ANCHOR_LEVEL_Y, label: ''
    stepper x: step_x, y: START_ANCHOR_SOURCE_Y, name: 'anchor-source', options: ANCHOR_SOURCES, selection: START_ANCHOR_SOURCE_SELECTION, width: STEPPER_WIDTH

    stepper x: step_x, y: END_ANCHOR_MODE_Y, name: 'anchor-mode', options: ANCHOR_MODES, selection: END_ANCHOR_MODE_SELECTION, width: STEPPER_WIDTH
    small_knob x: step_x, y: END_ANCHOR_LEVEL_Y, label: ''
    stepper x: step_x, y: END_ANCHOR_SOURCE_Y, name: 'anchor-source', options: ANCHOR_SOURCES, selection: END_ANCHOR_SOURCE_SELECTION, width: STEPPER_WIDTH

    stepper x: step_x, y: SHAPE_Y, name: 'shape', options: SHAPE_OPTIONS, selection: SHAPE_SELECTION, width: STEPPER_WIDTH
    small_knob x: step_x, y: CURVATURE_Y, label: ''

    small_knob y: DURATION_Y, x: step_x, label: ''

    button y: ENABLED_Y, x: step_x, label: ''
  end

  start_position_marker = step_block_x - Light::DIAMETER + STEP_WIDTH / 2.0
  end_position_marker = step_block_x + STEP_WIDTH * (steps - 1) + STEP_WIDTH / 2.0 + Light::DIAMETER

  position_marker(x: start_position_marker, y: PROGRESS_LIGHT_Y, type: :start)
  position_marker(x: end_position_marker, y: PROGRESS_LIGHT_Y, type: :end)
end

def position_marker(x:, y:, type:)
  marker = PositionMarker.new(type: type, color: @foreground)
  @controls << marker
  @image_shapes << marker.translated(x, y)
end
