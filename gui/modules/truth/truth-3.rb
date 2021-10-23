require_relative 'helpers/make-truth'

self.class::include Truth

HP = 10
CONDITION_Y = 20
OUTCOME_X = 40.64
PORT_DY = 15.5
INPUT_TOP = 76
INPUT_X = 10.28
OUTPUT_TOP = INPUT_TOP + PORT_DY
OUTPUT_X = 40.64

LAYOUT = {
  hp: HP,
  input_names: %w[A B C],
  input_state_combinations: [
    %w[F F F],
    %w[F F TStyle],
    %w[F TStyle F],
    %w[F TStyle TStyle],
    %w[TStyle F F],
    %w[TStyle F TStyle],
    %w[TStyle TStyle F],
    %w[TStyle TStyle TStyle],
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
