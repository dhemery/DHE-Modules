require_relative 'helpers/make-truth'

self.class::include Truth

CENTER_X = 20.48
OFFSET = 3.5
HP = 8
CONDITION_Y = 21
OUTCOME_X = 30.48
PORT_DY = 15.5
INPUT_TOP = 58
INPUT_X = CENTER_X - OFFSET
OUTPUT_TOP = INPUT_TOP + 2 * PORT_DY
OUTPUT_X = CENTER_X + OFFSET

LAYOUT = {
  hp: HP,
  input_names: %w[A B],
  input_state_combinations: [
    %w[F F],
    %w[F TStyle],
    %w[TStyle F],
    %w[TStyle TStyle],
  ],
  condition_y: CONDITION_Y,
  outcome_x: OUTCOME_X,
  input_top: INPUT_TOP,
  input_x: INPUT_X,
  output_top: OUTPUT_TOP,
  output_x: OUTPUT_X,
  port_dy: PORT_DY,
}

make_truth LAYOUT
