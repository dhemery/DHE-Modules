require_relative 'helpers/make-truth'

self.class::include Truth

HP = 16
WIDTH = hp2mm(HP)
CONDITION_Y = 23.66
OUTCOME_X = 71.11
PORT_DY = 14.585
INPUT_TOP = 29.36
INPUT_X = 10.28
OUTPUT_TOP = INPUT_TOP + 4 * PORT_DY

LAYOUT = {
  hp: HP,
  input_names: %w[A B C D],
  input_state_combinations: [
    %w[F F F F],
    %w[F F F TStyle],
    %w[F F TStyle F],
    %w[F F TStyle TStyle],
    %w[F TStyle F F],
    %w[F TStyle F TStyle],
    %w[F TStyle TStyle F],
    %w[F TStyle TStyle TStyle],
    %w[TStyle F F F],
    %w[TStyle F F TStyle],
    %w[TStyle F TStyle F],
    %w[TStyle F TStyle TStyle],
    %w[TStyle TStyle F F],
    %w[TStyle TStyle F TStyle],
    %w[TStyle TStyle TStyle F],
    %w[TStyle TStyle TStyle TStyle],
  ],
  condition_y: CONDITION_Y,
  outcome_x: OUTCOME_X,
  input_top: INPUT_TOP,
  input_x: INPUT_X,
  output_top: OUTPUT_TOP,
  output_x: INPUT_X + 7,
  port_dy: PORT_DY,
}

make_truth LAYOUT
