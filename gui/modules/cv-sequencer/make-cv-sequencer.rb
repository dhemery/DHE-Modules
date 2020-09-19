# frozen_string_literal: true

require_relative '../../lib/shapes/position-marker'
require_relative '../../lib/dimensions'

MARGIN_HP = 1.5
TOP_HP = 3.5
BOTTOM_HP = 23
LEFT_HP = MARGIN_HP
STEP_WIDTH_HP = 2.25
SEQUENCE_CONTROLS_WIDTH_HP = 9.0
STEP_BLOCK_LEFT_HP = LEFT_HP + SEQUENCE_CONTROLS_WIDTH_HP
GLOBAL_STEP_CONTROLS_WIDTH_HP = 4.0

MARGIN = hp2mm(MARGIN_HP)
TOP = hp2mm(TOP_HP)
BOTTOM = hp2mm(BOTTOM_HP)
LEFT = hp2mm(LEFT_HP)

HUE = 30
FOREGROUND_HSL = [HUE, 100, 10]
BACKGROUND_HSL = [HUE, 10, 93]

STEP_WIDTH = hp2mm(STEP_WIDTH_HP)

def step_block_width_hp(steps)
  steps * STEP_WIDTH_HP
end

def step_block_width(steps)
  steps * STEP_WIDTH
end

def step_block_right_hp(steps)
  STEP_BLOCK_LEFT_HP + step_block_width_hp(steps)
end

def right_hp(steps)
  step_block_right_hp(steps) + GLOBAL_STEP_CONTROLS_WIDTH_HP
end

def right(steps)
  hp2mm(right_hp(steps))
end

KNOB_BUTTON_DISTANCE = hp2mm(1.3)
BUTTON_BUTTON_DISTANCE = hp2mm(0.8)
ADVANCE_ANCHOR_DISTANCE = hp2mm(2.75)
ANCHOR_ANCHOR_DISTANCE = hp2mm(4.25)
ANCHOR_DURATION_DISTANCE = hp2mm(3.25)
DURATION_SHAPE_DISTANCE = hp2mm(3.25)

STEP_TOP = TOP + hp2mm(1.25)
PROGRESS_LIGHT_Y = TOP + Light::RADIUS * 1.5
TRIGGER_Y = TOP + hp2mm(1.61)
INTERRUPT_Y = TRIGGER_Y + BUTTON_BUTTON_DISTANCE
COMPLETION_Y = INTERRUPT_Y + BUTTON_BUTTON_DISTANCE
START_ANCHOR_Y = COMPLETION_Y + ADVANCE_ANCHOR_DISTANCE
END_ANCHOR_Y = START_ANCHOR_Y + ANCHOR_ANCHOR_DISTANCE
DURATION_Y = END_ANCHOR_Y + ANCHOR_DURATION_DISTANCE
SHAPE_Y = DURATION_Y + DURATION_SHAPE_DISTANCE
ENABLED_Y = BOTTOM - Button::DIAMETER / 2.0

STEPPER_WIDTH = STEP_WIDTH * 0.75
ANCHOR_SOURCES = %w[LEVEL IN OUT AUX]
ANCHOR_MODES = %w[SAMPLE TRACK]
STEP_LABEL_Y = TOP - hp2mm(0.25)

LINE_X_OFFSET = -STEP_WIDTH / 2.0

TRIGGER_MODES = %w[RISE FALL EDGE HIGH LOW]
TRIGGER_MODE_SELECTION = 1
INTERRUPT_MODES = %w[IGNORE NEXT]
INTERRUPT_MODE_SELECTION = 1
COMPLETION_MODES = %w[SUSTAIN NEXT]
COMPLETION_MODE_SELECTION = 1
SHAPE_OPTIONS = %w[J S]
SHAPE_SELECTION = 1

def cv_position_marker(x:, y:, type:)
  marker = PositionMarker.new(type: type, color: @foreground)
  @controls << marker
  @image_shapes << marker.translated(x, y)
end

def make_cv_sequencer(steps)

  name "CV SEQUENCER #{steps}"

  hp right_hp(steps)

  foreground FOREGROUND_HSL
  background BACKGROUND_HSL

  ###############################################################################
  #
  # Sequence Controls (Left)
  #
  ###############################################################################

  sequence_controls_top = TOP + hp2mm(2.78)
  sequence_controls_bottom = BOTTOM - Port::DIAMETER / 2.0 - 1.0
  sequence_controls_dy = (sequence_controls_bottom - sequence_controls_top) / 4
  run_y = sequence_controls_top + 0 * sequence_controls_dy
  loop_y = sequence_controls_top + 1 * sequence_controls_dy
  selection_y = sequence_controls_top + 2 * sequence_controls_dy
  gate_y = sequence_controls_top + 3 * sequence_controls_dy
  reset_y = sequence_controls_top + 4 * sequence_controls_dy

  selection_length_x_offset = hp2mm(1.63)
  selection_length_x = LEFT + selection_length_x_offset

  input_button_port x: LEFT, y: run_y, label: 'RUN'
  input_button_port x: LEFT, y: gate_y, label: 'GATE'
  connector left: LEFT, right: selection_length_x, y: selection_y
  small_knob x: LEFT - hp2mm(0.2), y: selection_y, label: 'START'
  small_knob x: selection_length_x, y: selection_y, label: 'LENGTH'
  input_button_port x: LEFT, y: loop_y, label: 'LOOP'
  input_button_port x: LEFT, y: reset_y, label: 'RESET'


  ###############################################################################
  #
  # Step Controls
  #
  ###############################################################################

  step_left = hp2mm(8.5)

  label_x = step_left - 0.6 * STEP_WIDTH
  label x: label_x, y: TRIGGER_Y, text: 'TRIG', alignment: :left_of, size: :large
  label x: label_x, y: INTERRUPT_Y, text: 'INT', alignment: :left_of, size: :large
  label x: label_x, y: COMPLETION_Y, text: 'AT END', alignment: :left_of, size: :large
  label x: label_x, y: START_ANCHOR_Y, text: 'START', alignment: :left_of, size: :large
  label x: label_x, y: END_ANCHOR_Y, text: 'END', alignment: :left_of, size: :large
  label x: label_x, y: SHAPE_Y - hp2mm(0.25), text: 'SHAPE', alignment: :left_of, size: :large
  label x: label_x, y: DURATION_Y, text: 'DUR', alignment: :left_of, size: :large
  label x: label_x, y: ENABLED_Y, text: 'ON', alignment: :left_of, size: :large

  step_block(steps, step_left)


  ###############################################################################
  #
  # Sequence Controls (Right)
  #
  ###############################################################################

  global_left_x = right(steps) - hp2mm(5)
  global_right_x = right(steps) - hp2mm(2)
  out_y = BOTTOM - Port::DIAMETER / 2.0 - 1.0
  in_y = sequence_controls_top
  polarity_y = (START_ANCHOR_Y + END_ANCHOR_Y) / 2.0

  input_port x: global_left_x, y: in_y, label: 'IN'
  input_port x: global_right_x, y: in_y, label: 'AUX'
  polarity_toggle x: global_left_x, y: polarity_y, selection: 2
  duration_toggle x: global_left_x, y: DURATION_Y
  port x: global_right_x, y: DURATION_Y, label: 'CV'
  output_port x: global_right_x, y: out_y, label: 'OUT'
end

def position_marker(x:, y:, type:)
  marker = PositionMarker.new(type: type, color: @foreground)
  @controls << marker
  @image_shapes << marker.translated(x, y)
end

def step_block(steps, left)
  (0...steps).each do |step|
    x = left + step * STEP_WIDTH
    if step != 0
      # Skip the line for the first step.
      line_x = x + LINE_X_OFFSET
      line x1: line_x, x2: line_x, y1: STEP_TOP, y2: BOTTOM
    end

    light x: x, y: PROGRESS_LIGHT_Y
    label x: x, y: STEP_LABEL_Y, text: (step + 1).to_s, alignment: :above, size: :large

    stepper x: x, y: TRIGGER_Y, name: 'trigger-mode', options: TRIGGER_MODES, selection: TRIGGER_MODE_SELECTION, width: STEPPER_WIDTH
    stepper x: x, y: INTERRUPT_Y, name: 'interrupt-mode', options: INTERRUPT_MODES, selection: INTERRUPT_MODE_SELECTION, width: STEPPER_WIDTH
    stepper x: x, y: COMPLETION_Y, name: 'completion-mode', options: COMPLETION_MODES, selection: COMPLETION_MODE_SELECTION, width: STEPPER_WIDTH

    stepper x: x, y: START_ANCHOR_Y - KNOB_BUTTON_DISTANCE, name: 'anchor-mode', options: ANCHOR_MODES, selection: 1, width: STEPPER_WIDTH
    small_knob x: x, y: START_ANCHOR_Y, label: ''
    stepper x: x, y: START_ANCHOR_Y + KNOB_BUTTON_DISTANCE, name: 'anchor-source', options: ANCHOR_SOURCES, selection: 3, width: STEPPER_WIDTH

    stepper x: x, y: END_ANCHOR_Y - KNOB_BUTTON_DISTANCE, name: 'anchor-mode', options: ANCHOR_MODES, selection: 2, width: STEPPER_WIDTH
    small_knob x: x, y: END_ANCHOR_Y, label: ''
    stepper x: x, y: END_ANCHOR_Y + KNOB_BUTTON_DISTANCE, name: 'anchor-source', options: ANCHOR_SOURCES, selection: 1, width: STEPPER_WIDTH

    stepper x: x, y: SHAPE_Y - KNOB_BUTTON_DISTANCE, name: 'shape', options: SHAPE_OPTIONS, selection: SHAPE_SELECTION, width: STEPPER_WIDTH
    small_knob x: x, y: SHAPE_Y, label: ''

    small_knob y: DURATION_Y, x: x, label: ''

    button y: ENABLED_Y, x: x, label: ''
  end

  start_position_marker = left - Light::DIAMETER
  end_position_marker = left + STEP_WIDTH * (steps - 1) + Light::DIAMETER

  position_marker(x: start_position_marker, y: PROGRESS_LIGHT_Y, type: :start)
  position_marker(x: end_position_marker, y: PROGRESS_LIGHT_Y, type: :end)
end
