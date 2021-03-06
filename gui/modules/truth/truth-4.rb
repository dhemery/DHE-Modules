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
    %w[F F F T],
    %w[F F T F],
    %w[F F T T],
    %w[F T F F],
    %w[F T F T],
    %w[F T T F],
    %w[F T T T],
    %w[T F F F],
    %w[T F F T],
    %w[T F T F],
    %w[T F T T],
    %w[T T F F],
    %w[T T F T],
    %w[T T T F],
    %w[T T T T],
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
