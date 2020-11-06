# frozen_string_literal: true

require_relative '../../lib/dimensions'

module Scannable
  HUE = 30
  FOREGROUND_HSL = [HUE, 100, 10]
  BACKGROUND_HSL = [HUE, 10, 93]

  STEP_WIDTH_HP = 2.25
  BASE_WIDTH_HP = 11

  SECTIONS = 3

  def make(steps)
    width_hp = BASE_WIDTH_HP + steps * STEP_WIDTH_HP
    name "SCANNABLE #{steps}"
    hp width_hp
    foreground FOREGROUND_HSL
    background BACKGROUND_HSL

    width_mm = hp2mm(width_hp)
    content_width_mm = content_width(steps)
    excess_width = width_mm - PADDING - content_width_mm
    margin = excess_width / (SECTIONS + 1.0)

    global_inputs_left = margin
    make_global_inputs(global_inputs_left)

    labels_left = global_inputs_left + GLOBAL_CONTROLS_WIDTH + margin
    make_step_control_labels(labels_left)

    step_block_left = labels_left + LABELS_WIDTH + PADDING
    make_step_block(steps, step_block_left)

    global_outputs_left = step_block_left + step_block_width(steps) + margin
    make_global_outputs(global_outputs_left)
  end

  TOP = 23
  BOTTOM = 117.0
  STEP_WIDTH = hp2mm(STEP_WIDTH_HP)

  GLOBAL_CONTROLS_WIDTH = PADDING + Port::DIAMETER + PADDING
  LABELS_WIDTH = 10.0

  def step_block_width(steps)
    steps * STEP_WIDTH
  end

  def content_width(steps)
    GLOBAL_CONTROLS_WIDTH + LABELS_WIDTH + step_block_width(steps) + GLOBAL_CONTROLS_WIDTH
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

  def vertical(x)
    line x1: x, x2: x, y1: TOP, y2: BOTTOM
  end

  def horizontal(y)
    line x1: 0, x2: width, y1: y, y2: y
  end

  def make_global_inputs(left)
    x = left + Port::RADIUS + PADDING

    steps_y = global_controls_y(0)
    a_y = global_controls_y(1)
    b_y = global_controls_y(2)
    c_y = global_controls_y(3)
    phase_in_y = global_controls_y(4)

    small_knob x: x, y: steps_y, label: 'STEPS'
    input_port x: x, y: a_y, label: 'A'
    input_port x: x, y: b_y, label: 'B'
    input_port x: x, y: c_y, label: 'C'
    input_port x: x, y: phase_in_y, label: 'PHASE'
  end

  def make_global_outputs(left)
    x = left + Port::RADIUS + PADDING

    polarity_y = global_controls_y(0)
    step_number_y = global_controls_y(2)
    step_phase_y = global_controls_y(3)
    out_y = global_controls_y(4)

    polarity_toggle x: x, y: polarity_y, selection: 2
    output_port x: x, y: step_number_y, label: 'STEP'
    output_port x: x, y: step_phase_y, label: 'STEP ϕ'
    output_port x: x, y: out_y, label: 'OUT'
  end

  STEPPER_WIDTH = Knob::DIAMETERS[:small]
  # The distance from the stepper baseline to the top of its box
  STEPPER_ASCENT = Label::SIZES[:small] / 2.0 + PADDING - 0.25
  STEPPER_HEIGHT = STEPPER_ASCENT * 2.0

  INTRA_SECTION_GLUE = 0.15
  INTER_SECTION_GLUE = 2.0

  PROGRESS_LIGHT_Y = TOP - Light::DIAMETER * 1.5
  STEP_NUMBER_LABEL_Y = PROGRESS_LIGHT_Y - Light::DIAMETER * 1.5

  START_ANCHOR_MODE_Y = TOP + STEPPER_ASCENT
  START_ANCHOR_SOURCE_Y = START_ANCHOR_MODE_Y + (STEPPER_HEIGHT + STEPPER_HEIGHT) / 2.0 + INTRA_SECTION_GLUE
  START_ANCHOR_LEVEL_Y = START_ANCHOR_SOURCE_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE
  START_ANCHOR_LEVEL_CV_Y = START_ANCHOR_LEVEL_Y + (Knob::DIAMETERS[:small] + Port::DIAMETER) / 2.0 + INTRA_SECTION_GLUE

  END_ANCHOR_MODE_Y = START_ANCHOR_LEVEL_CV_Y + (Port::DIAMETER + STEPPER_HEIGHT) / 2.0 + INTER_SECTION_GLUE
  END_ANCHOR_SOURCE_Y = END_ANCHOR_MODE_Y + (STEPPER_HEIGHT + STEPPER_HEIGHT) / 2.0 + INTRA_SECTION_GLUE
  END_ANCHOR_LEVEL_Y = END_ANCHOR_SOURCE_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE
  END_ANCHOR_LEVEL_CV_Y = END_ANCHOR_LEVEL_Y + (Knob::DIAMETERS[:small] + Port::DIAMETER) / 2.0 + INTRA_SECTION_GLUE

  DURATION_Y = END_ANCHOR_LEVEL_CV_Y + (Port::DIAMETER + Knob::DIAMETERS[:small]) / 2.0 + INTER_SECTION_GLUE
  DURATION_CV_Y = DURATION_Y + (Knob::DIAMETERS[:small] + Port::DIAMETER) / 2.0 + INTRA_SECTION_GLUE

  SHAPE_Y = DURATION_CV_Y + (Port::DIAMETER + STEPPER_HEIGHT) / 2.0 + INTER_SECTION_GLUE
  CURVATURE_Y = SHAPE_Y + (STEPPER_HEIGHT + Knob::DIAMETERS[:small]) / 2.0 + INTRA_SECTION_GLUE
  CURVATURE_CV_Y = CURVATURE_Y + (Knob::DIAMETERS[:small] + Port::DIAMETER) / 2.0 + INTRA_SECTION_GLUE

  START_ANCHOR_LABEL_Y = (START_ANCHOR_SOURCE_Y + START_ANCHOR_LEVEL_CV_Y) / 2.0
  END_ANCHOR_LABEL_Y = (END_ANCHOR_SOURCE_Y + END_ANCHOR_LEVEL_CV_Y) / 2.0
  DURATION_LABEL_Y = (DURATION_Y + DURATION_CV_Y) / 2.0
  SHAPE_LABEL_Y = (SHAPE_Y + CURVATURE_CV_Y) / 2.0

  def make_step_control_labels(left)
    x = left + LABELS_WIDTH
    label x: x, y: START_ANCHOR_LABEL_Y, text: 'START', alignment: :left_of, size: :large
    label x: x, y: END_ANCHOR_LABEL_Y, text: 'END', alignment: :left_of, size: :large
    label x: x, y: DURATION_LABEL_Y, text: 'ϕ DUR', alignment: :left_of, size: :large
    label x: x, y: SHAPE_LABEL_Y - hp2mm(0.25), text: 'SHAPE', alignment: :left_of, size: :large
  end

  ANCHOR_SOURCES = %w[LEVEL A B C OUT]
  ANCHOR_MODES = %w[SMPL TRACK]
  END_ANCHOR_MODE_SELECTION = 2
  END_ANCHOR_SOURCE_SELECTION = 1
  SHAPE_OPTIONS = %w[J S]
  SHAPE_SELECTION = 1
  START_ANCHOR_MODE_SELECTION = 1
  START_ANCHOR_SOURCE_SELECTION = 5

  def make_step_block(steps, left)
    line x1: left, x2: left, y1: TOP, y2: BOTTOM

    (0...steps).each do |step|
      step_left = left + step * STEP_WIDTH
      x = step_left + STEP_WIDTH / 2.0
      step_right = step_left + STEP_WIDTH
      line x1: step_right, x2: step_right, y1: TOP, y2: BOTTOM

      light x: x, y: PROGRESS_LIGHT_Y
      label x: x, y: STEP_NUMBER_LABEL_Y, text: (step + 1).to_s, alignment: :above, size: :large

      stepper x: x, y: START_ANCHOR_MODE_Y, name: 'anchor-mode', options: ANCHOR_MODES, selection: START_ANCHOR_MODE_SELECTION, width: STEPPER_WIDTH
      stepper x: x, y: START_ANCHOR_SOURCE_Y, name: 'anchor-source', options: ANCHOR_SOURCES, selection: START_ANCHOR_SOURCE_SELECTION, width: STEPPER_WIDTH
      small_knob x: x, y: START_ANCHOR_LEVEL_Y, label: ''
      port x: x, y: START_ANCHOR_LEVEL_CV_Y, label: ''

      stepper x: x, y: END_ANCHOR_SOURCE_Y, name: 'anchor-source', options: ANCHOR_SOURCES, selection: END_ANCHOR_SOURCE_SELECTION, width: STEPPER_WIDTH
      stepper x: x, y: END_ANCHOR_MODE_Y, name: 'anchor-mode', options: ANCHOR_MODES, selection: END_ANCHOR_MODE_SELECTION, width: STEPPER_WIDTH
      small_knob x: x, y: END_ANCHOR_LEVEL_Y, label: ''
      port x: x, y: END_ANCHOR_LEVEL_CV_Y, label: ''

      small_knob x: x, y: DURATION_Y, label: ''
      port x: x, y: DURATION_CV_Y, label: ''

      stepper x: x, y: SHAPE_Y, name: 'shape', options: SHAPE_OPTIONS, selection: SHAPE_SELECTION, width: STEPPER_WIDTH
      small_knob x: x, y: CURVATURE_Y, label: ''
      port x: x, y: CURVATURE_CV_Y, label: ''
    end

    start_position_marker = left - Light::DIAMETER + STEP_WIDTH / 2.0
    end_position_marker = left + STEP_WIDTH * (steps - 1) + STEP_WIDTH / 2.0 + Light::DIAMETER

    position_marker(x: start_position_marker, y: PROGRESS_LIGHT_Y, type: :start)
    position_marker(x: end_position_marker, y: PROGRESS_LIGHT_Y, type: :end)
  end

  def position_marker(x:, y:, type:)
    marker = PositionMarker.new(type: type, color: @foreground)
    @controls << marker
    @image_shapes << marker.translated(x, y)
  end
end
